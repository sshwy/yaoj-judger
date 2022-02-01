#include <linux/seccomp.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "../kafel/include/kafel.h"
#include "builtin_hook.h"
#include "common.h"
#include "hook.h"

static struct timeval start, end;

static void timer_before_fork(struct perform_ctxt *ctxt) {
  LOG_INFO("get start time.\n");
  gettimeofday(&start, NULL);
}

static void timer_after_wait(struct perform_ctxt *ctxt) {
  gettimeofday(&end, NULL);
  ctxt->result.real_time = (int)(end.tv_sec * 1000 + end.tv_usec / 1000 -
                                 start.tv_sec * 1000 - start.tv_usec / 1000);

  LOG_INFO("get end time, real time: %.3lfs\n",
           ctxt->result.real_time / 1000.0);
}

static void init_result_before_fork(struct perform_ctxt *ctxt) {
  ctxt->result.code = SE;
  ctxt->result.exit_code = 0;
  ctxt->result.signal = 0;
  ctxt->result.real_memory = 0;
  ctxt->result.real_time = 0;

  LOG_INFO("init ctxt before fork.\n");
}

static void analyze_before_return(struct perform_ctxt *ctxt) {
  ctxt->result.real_memory = ctxt->rusage.ru_maxrss;

  if (WIFSIGNALED(ctxt->status)) {
    LOG_INFO("child process terminated by signal %d.\n",
             WTERMSIG(ctxt->status));

    ctxt->result.signal = WTERMSIG(ctxt->status);

    switch (WTERMSIG(ctxt->status)) {
    case SIGUSR1:
      ctxt->result.code = SE;
      break;
    case SIGSYS:
      ctxt->result.code = DSC;
      break;
    case SIGXCPU:
      ctxt->result.code = TLE;
      break;
    case SIGSEGV:
      if (ctxt->rctxt->virtual_memory == RSC_UNLIMITED)
        ctxt->result.code = RE;
      else
        ctxt->result.code = MLE;
      break;
    case SIGXFSZ:
      ctxt->result.code = OLE;
      break;
    case SIGKILL:
      if (ctxt->rctxt->time != RSC_UNLIMITED &&
          ctxt->result.real_time > ctxt->rctxt->time)
        ctxt->result.code = TLE;
      else
        ctxt->result.code = SE; // someone unknown killed it
      break;
    default:
      ctxt->result.code = OK;
    }

  } else {
    ASSERT(WIFEXITED(ctxt->status), "assertion failed.\n");
    LOG_INFO("child process exited with code %d.\n", WEXITSTATUS(ctxt->status));

    ctxt->result.exit_code = WEXITSTATUS(ctxt->status);
    if (WEXITSTATUS(ctxt->status) == 0) {
      if (ctxt->rctxt->actual_memory != RSC_UNLIMITED &&
          ctxt->result.real_memory * KB > ctxt->rctxt->actual_memory)
        ctxt->result.code = MLE;
      else if (ctxt->rctxt->time != RSC_UNLIMITED &&
               ctxt->result.real_time > ctxt->rctxt->time)
        ctxt->result.code = TLE;
      else
        ctxt->result.code = OK;
    } else {
      ctxt->result.code = ECE;
    }
  }
  LOG_INFO("analyzed result.\n");
}

void compile_policy_before_fork(struct perform_ctxt *ctxt) {
  char file[200];
  strcpy(file, ctxt->pctxt->policy);
  strcat(file, ".policy");
  const char *policy_search_path = ctxt->pctxt->dirname;
  char *filename = path_join(ctxt->pctxt->dirname, file);

  struct sock_fprog prog;

  kafel_ctxt_t kctxt = kafel_ctxt_create();
  char *s = ftos(fopen(filename, "rb"));

  kafel_set_input_string(kctxt, s);
  kafel_add_include_search_path(kctxt, policy_search_path);

  if (kafel_compile(kctxt, &prog)) {
    ERRNO_EXIT(-1, "policy compilation failed: %s", kafel_error_msg(kctxt));
  }

  kafel_ctxt_destroy(&kctxt);

  LOG_INFO("compile policy \"%s\" succeed.\n", filename);

  ctxt->prog = prog;
}

void register_builtin_hook(struct hook_ctxt *hctxt) {
  // timer_before_fork should be the lastone to invoke
  register_hook(hctxt, BEFORE_FORK, timer_before_fork);
  register_hook(hctxt, BEFORE_FORK, compile_policy_before_fork);
  register_hook(hctxt, BEFORE_FORK, init_result_before_fork);

  register_hook(hctxt, AFTER_WAIT, timer_after_wait);
  register_hook(hctxt, BEFORE_RETURN, analyze_before_return);

  LOG_INFO("register builtin hook. (%d)\n", getpid());
}