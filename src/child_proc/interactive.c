#include <unistd.h>

#include "common.h"
#include "lib/policy.h"
#include "runner.h"
#include "tackle.h"

int ptoc[2], ctop[2];

static perform_ctxt_t ctxt;

static void ta_before_fork(pid_t self) {
  ASSERT(pipe(ptoc) == 0 && pipe(ctop) == 0, "create pipe failed\n");
}

static void ta_child_prework(pid_t self) {
  ASSERT(dup2(ptoc[1], fileno(stdin)) != -1, "dup2 failed.\n");
  ASSERT(dup2(ctop[0], fileno(stdout)) != -1, "dup2 failed.\n");
  prework(ctxt->ectxt);
  apply_resource_limit(ctxt->rctxt);
  apply_policy(ctxt);
  fflush(log_fp);
  fclose(log_fp); // !!!important
}

static void ta_after_fork(pid_t self, pid_t child) {
  // interactor
  ASSERT(dup2(ctop[1], fileno(stdin)) != -1, "dup2 failed.\n");
  ASSERT(dup2(ptoc[0], fileno(stdout)) != -1, "dup2 failed.\n");
  apply_policy(ctxt); // apply the same policy for interactor
  fflush(log_fp);
  fclose(log_fp); // !!!important
}

static void ta_child_run(pid_t self

void child_prework(perform_ctxt_t _ctxt) {

}

void child_run(perform_ctxt_t _ctxt) {
  ctxt = _ctxt;
  struct tackle_ctxt tactxt = {
      .before_fork = ta_before_fork,
      .child_prework = ta_child_prework,
      .after_fork = ta_after_fork,
      .child_run = ta_child_run,
  };
  tackle(&tactxt);
}