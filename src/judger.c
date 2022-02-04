#include <linux/seccomp.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/wait.h>

#include "builtin_hook.h"
#include "common.h"
#include "hook.h"
#include "judger.h"
#include "lib/policy.h"
#include "lib/resouce_limit.h"
#include "lib/tkill.h"
#include "runner.h"
#include "tackle.h"

FILE *log_fp;

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
  apply_resource_limit(ctxt->rctxt);
  apply_policy(ctxt->pctxt->prog);
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
