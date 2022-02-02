#include <linux/seccomp.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/wait.h>

#include "builtin_hook.h"
#include "common.h"
#include "hook.h"
#include "judger.h"
#include "policy.h"
#include "runner/runner.h"
#include "tkill.h"

FILE *log_fp;

int apply_policy_child(struct sock_fprog prog) {
  ASSERT(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == 0,
         "error applying policy.\n");
  ASSERT(prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0) == 0,
         "error applying policy.\n");

  free(prog.filter);
  LOG_INFO("apply policy succeed.\n");
  return EXIT_SUCCESS;
}

void set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  const struct rlimit rl = {.rlim_cur = cur, .rlim_max = max};
  setrlimit(type, &rl);
}

void apply_resource_child(const struct rsclim_ctxt *rctxt) {
  ASSERT(rctxt->time >= 0, "invalid cpu_time\n");
  ASSERT(rctxt->virtual_memory >= 0, "invalid virtual_memory\n");
  ASSERT(rctxt->actual_memory >= 0, "invalid actual_memory\n");
  ASSERT(rctxt->output_size >= 0, "invalid output_size\n");
  ASSERT(rctxt->stack_memory >= 0, "invalid stack_memory\n");

  if (rctxt->time > 0) {
    // upper bound
    int time_ins = rctxt->time / 1000 + !!(rctxt->time % 1000);
    // 实际上只限制了 CPU 的时间，可以看作一个 hard limit，真要干还是得开线程
    set_rlimit(RLIMIT_CPU, time_ins, CPU_TIME_H_LIMIT);
    LOG_INFO("set cpu time (RLIMIT_CPU): %ds\n", time_ins);
  }

  if (rctxt->virtual_memory > 0) {
    // RLIMIT_AS 限制的是虚拟内存的大小，而不是实际用量。比如可以实现 CCF
    // feature：数组开爆
    set_rlimit(RLIMIT_AS, rctxt->virtual_memory,
               max(rctxt->virtual_memory, AS_H_LIMIT));
    LOG_INFO("set memory limit: %.3lf KB\n", rctxt->virtual_memory * 1.0 / KB);
  }

  if (rctxt->output_size > 0) {
    set_rlimit(RLIMIT_FSIZE, rctxt->output_size,
               max(rctxt->output_size, FSIZE_H_LIMIT));
    LOG_INFO("set output limit: %.3lf KB\n", rctxt->output_size * 1.0 / KB);
  }

  if (rctxt->stack_memory > 0) {
    set_rlimit(RLIMIT_STACK, rctxt->stack_memory,
               max(rctxt->stack_memory, STACK_H_LIMIT));
    LOG_INFO("set stack memory limit: %.3lf KB\n",
             rctxt->stack_memory * 1.0 / KB);
  }
}

void perform_child(struct perform_ctxt *ctxt) {
  LOG_INFO("perform child (%d)\n", getpid());
  // struct sock_fprog prog = compile_policy(ctxt->pctxt);
  prework(ctxt->ectxt);
  apply_resource_child(ctxt->rctxt);
  ASSERT(apply_policy_child(ctxt->pctxt->prog) == 0,
         "perform: apply policy child error\n");
  fclose(log_fp); // !!!important
  run(ctxt->ectxt);
}

void perform(struct perform_ctxt *ctxt, struct policy_ctxt pctxt,
             struct rsclim_ctxt rctxt, struct runner_ctxt ectxt,
             struct hook_ctxt hctxt) {
  ctxt->pctxt = &pctxt;
  ctxt->rctxt = &rctxt;
  ctxt->ectxt = &ectxt;
  ctxt->hctxt = &hctxt;

  runner_hook(ctxt);
  register_builtin_hook(&hctxt);
  run_hook_chain(hctxt.before_fork, ctxt);

  compile_policy(&pctxt, ctxt);

  fflush(log_fp); // avoid multi logging
  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.");

  if (child_pid == 0) {
    perform_child(ctxt);
    exit(EXIT_FAILURE);
  }
  run_hook_chain(hctxt.after_fork, ctxt);

  pthread_t tid = 0;
  struct tkill_ctxt tctxt = {.pid = child_pid, .time = rctxt.time};
  if (rctxt.time != RSC_UNLIMITED)
    tid = start_timeout_killer(&tctxt);

  if (wait4(child_pid, &ctxt->status, WSTOPPED, &ctxt->rusage) == -1) {
    kill(child_pid, SIGKILL);
    ERRNO_EXIT(-1, "wait failed.\n");
  }
  run_hook_chain(hctxt.after_wait, ctxt);

  if (rctxt.time != RSC_UNLIMITED)
    stop_timeout_killer(tid);

  run_hook_chain(hctxt.before_return, ctxt);

  ctxt->pctxt = NULL;
  ctxt->rctxt = NULL;
  ctxt->ectxt = NULL;
  ctxt->hctxt = NULL;
  LOG_INFO("judge finished.\n");
}
