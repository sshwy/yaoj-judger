#include <linux/seccomp.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/wait.h>

#include "builtin_hook.h"
#include "common.h"
#include "hook.h"
#include "judger.h"
#include "policy.h"
#include "runner.h"
#include "tackle.h"
#include "tkill.h"

FILE *log_fp;

int apply_policy_child(struct sock_fprog prog) {
  ASSERT(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == 0,
         "error applying policy.\n");
  ASSERT(prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0) == 0,
         "error applying policy.\n");

  free(prog.filter);
  LOG_INFO("apply policy succeed.\n");
  return 0;
}

void set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  const struct rlimit rl = {.rlim_cur = cur, .rlim_max = max};
  ASSERT(setrlimit(type, &rl) == 0, "setrlimit failed.\n");
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

static struct perform_ctxt *ctxt;
static pthread_t tid;

void ta_before_fork(pid_t self) {
  register_builtin_hook(ctxt->hctxt);
  run_hook_chain(ctxt->hctxt->before_fork, ctxt);

  compile_policy(ctxt->pctxt, ctxt);

  fflush(log_fp); // avoid multi logging
}
void ta_child_proc(pid_t self) {
  LOG_INFO("perform child (%d)\n", self);
  // struct sock_fprog prog = compile_policy(ctxt->pctxt);
  prework(ctxt->ectxt);
  apply_resource_child(ctxt->rctxt);
  ASSERT(apply_policy_child(ctxt->pctxt->prog) == 0,
         "perform: apply policy child error\n");
  fflush(log_fp);
  fclose(log_fp); // !!!important
  run(ctxt->ectxt);
}
void ta_after_fork(pid_t self, pid_t child_pid) {
  LOG_INFO("parent (%d) child (%d)\n", self, child_pid);
  run_hook_chain(ctxt->hctxt->after_fork, ctxt);

  tid = 0;
  struct tkill_ctxt tctxt = {.pid = child_pid, .time = ctxt->rctxt->time};
  if (ctxt->rctxt->time != RSC_UNLIMITED)
    tid = start_timeout_killer(&tctxt);
}
void ta_after_wait(pid_t self, pid_t child, int status) {
  ctxt->status = status;
  struct rusage r2;
  ASSERT(getrusage(RUSAGE_CHILDREN, &r2) == 0, "test failed.");
  fprint_rusage(log_fp, &r2);
  ctxt->rusage = r2;

  run_hook_chain(ctxt->hctxt->after_wait, ctxt);

  if (ctxt->rctxt->time != RSC_UNLIMITED)
    stop_timeout_killer(tid);

  run_hook_chain(ctxt->hctxt->before_return, ctxt);

  LOG_INFO("judge finished.\n");
}

void perform(struct perform_ctxt *_ctxt) {
  ctxt = _ctxt;
  struct tackle_ctxt tactxt = {
      .before_fork = ta_before_fork,
      .child_proc = ta_child_proc,
      .after_fork = ta_after_fork,
      .after_wait = ta_after_wait,
  };
  tackle(&tactxt);
}
