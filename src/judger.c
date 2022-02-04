#include "judger.h"
#include "builtin_hook.h"
#include "child_proc.h"
#include "common.h"
#include "hook.h"
#include "tackle.h"

static perform_ctxt_t ctxt;

static void ta_before_fork(pid_t self) {
  ctxt->pself = self;
  register_builtin_hook(ctxt->hctxt);
  run_hook_chain(ctxt->hctxt->before_fork, ctxt);
  fflush(log_fp); // avoid multi logging
}

static void ta_child_proc(pid_t self) {
  ctxt->pchild = self;
  child_proc(ctxt);
}

static void ta_after_fork(pid_t self, pid_t child_pid) {
  LOG_INFO("parent (%d) child (%d)\n", self, child_pid);
  ctxt->pchild = child_pid;
  run_hook_chain(ctxt->hctxt->after_fork, ctxt);
}

static void ta_after_wait(pid_t self, pid_t child, int status) {
  ctxt->status = status;
  run_hook_chain(ctxt->hctxt->after_wait, ctxt);
  LOG_INFO("judge finished.\n");
}

void perform(perform_ctxt_t _ctxt) {
  ctxt = _ctxt;
  struct tackle_ctxt tactxt = {
      .before_fork = ta_before_fork,
      .child_proc = ta_child_proc,
      .after_fork = ta_after_fork,
      .after_wait = ta_after_wait,
  };
  tackle(&tactxt);
}
