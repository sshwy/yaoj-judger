#include <linux/seccomp.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "common.h"
#include "judger.h"
#include "runner/runner.h"
#include "tkill.h"

FILE *log_fp;

char *filename;

int max(int a, int b) { return a > b ? a : b; }

struct sock_fprog compile_policy_child(struct policy_ctxt pctxt) {
  char file[200];
  strcpy(file, pctxt.policy);
  strcat(file, ".policy");
  const char *policy_search_path = pctxt.dirname;
  filename = path_join(pctxt.dirname, file);

  struct sock_fprog prog;

  kafel_ctxt_t ctxt = kafel_ctxt_create();
  kafel_set_input_file(ctxt, fopen(filename, "r"));
  kafel_add_include_search_path(ctxt, policy_search_path);

  if (kafel_compile(ctxt, &prog)) {
    ERRNO_EXIT(-1, "policy compilation failed: %s", kafel_error_msg(ctxt));
  }

  kafel_ctxt_destroy(&ctxt);

  LOG_INFO("compile policy \"%s\" succeed.\n", filename);

  return prog;
}

int apply_policy_child(struct sock_fprog prog) {
  ASSERT(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == 0,
         "error applying policy.\n");
  ASSERT(prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0) == 0,
         "error applying policy.\n");

  free(prog.filter);
  LOG_INFO("apply policy \"%s\" succeed.\n", filename);
  return EXIT_SUCCESS;
}

void set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  const struct rlimit rl = {.rlim_cur = cur, .rlim_max = max};
  setrlimit(type, &rl);
}
void apply_resource_child(struct rsclim_ctxt rctxt) {
  ASSERT(rctxt.time >= 0, "invalid cpu_time\n");
  ASSERT(rctxt.virtual_memory >= 0, "invalid virtual_memory\n");
  ASSERT(rctxt.actual_memory >= 0, "invalid actual_memory\n");
  ASSERT(rctxt.output_size >= 0, "invalid output_size\n");

  if (rctxt.time > 0) {
    // upper bound
    int time_insecond = rctxt.time / 1000 + !!(rctxt.time % 1000);
    // 实际上只限制了 CPU 的时间，可以看作一个 hard limit，真要干还是得开线程
    set_rlimit(RLIMIT_CPU, time_insecond, CPU_TIME_H_LIMIT);
    LOG_INFO("set cpu time (RLIMIT_CPU): %ds\n", time_insecond);
  }

  if (rctxt.virtual_memory > 0) {
    // RLIMIT_AS 限制的是虚拟内存的大小，而不是实际用量。比如可以实现 CCF
    // feature：数组开爆
    set_rlimit(RLIMIT_AS, rctxt.virtual_memory,
               max(rctxt.virtual_memory, AS_H_LIMIT));
    LOG_INFO("set memory limit: %.3lf KB\n", rctxt.virtual_memory * 1.0 / KB);
  }

  if (rctxt.output_size > 0) {
    set_rlimit(RLIMIT_FSIZE, rctxt.output_size,
               max(rctxt.output_size, FSIZE_H_LIMIT));
    LOG_INFO("set output limit: %.3lf KB\n", rctxt.output_size * 1.0 / KB);
  }
}

void perform_child(struct policy_ctxt pctxt, struct rsclim_ctxt rctxt,
                   struct runner_ctxt ectxt) {
  struct sock_fprog prog = compile_policy_child(pctxt);
  ASSERT(prework(ectxt) == 0, "perform: prework error.\n");
  apply_resource_child(rctxt);
  ASSERT(apply_policy_child(prog) == 0, "perform: apply policy child error\n");
  fclose(log_fp); // !!!important
  run(ectxt);
}

struct result perform(struct policy_ctxt pctxt, struct rsclim_ctxt rctxt,
                      struct runner_ctxt ectxt) {
  struct timeval start, end;
  gettimeofday(&start, NULL);

  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.");

  if (child_pid == 0) {
    perform_child(pctxt, rctxt, ectxt);
    exit(EXIT_FAILURE);
  }

  pthread_t tid = 0;
  struct tkill_ctxt tctxt = {.pid = child_pid, .time = rctxt.time};
  if (rctxt.time != RSC_UNLIMITED)
    tid = start_timeout_killer(&tctxt);

  int status;
  struct rusage resource_usage;

  if (wait4(child_pid, &status, WSTOPPED, &resource_usage) == -1) {
    kill(child_pid, SIGKILL);
    ERRNO_EXIT(-1, "wait failed.\n");
  }

  if (rctxt.time != RSC_UNLIMITED)
    stop_timeout_killer(tid);

  gettimeofday(&end, NULL);

  struct result result = {
      .exit_code = SE,
      .code = 0,
      .signal = 0,
      .real_memory = resource_usage.ru_maxrss,
      .real_time = (int)(end.tv_sec * 1000 + end.tv_usec / 1000 -
                         start.tv_sec * 1000 - start.tv_usec / 1000)};

  if (WIFSIGNALED(status)) {
    LOG_INFO("child process terminated by signal %d.\n", WTERMSIG(status));

    result.signal = WTERMSIG(status);

    switch (WTERMSIG(status)) {
    case SIGUSR1:
      result.code = SE;
      break;
    case SIGSYS:
      result.code = DSC;
      break;
    case SIGXCPU:
      result.code = TLE;
      break;
    case SIGSEGV:
      if (rctxt.virtual_memory == RSC_UNLIMITED)
        result.code = RE;
      else
        result.code = MLE;
      break;
    case SIGXFSZ:
      result.code = OLE;
      break;
    case SIGKILL:
      if (rctxt.time != RSC_UNLIMITED && result.real_time > rctxt.time)
        result.code = TLE;
      else
        result.code = SE; // someone unknown killed it
      break;
    default:
      result.code = OK;
    }

  } else {
    ASSERT(WIFEXITED(status), "assertion failed.\n");
    LOG_INFO("child process exited with code %d.\n", WEXITSTATUS(status));

    result.exit_code = WEXITSTATUS(status);

    switch (WEXITSTATUS(status)) {}
  }

  LOG_INFO("judge finished.\n");

  return result;
}
