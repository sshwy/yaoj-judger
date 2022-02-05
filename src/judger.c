#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin_hook.h"
#include "child_proc.h"
#include "common.h"
#include "hook.h"
#include "judger.h"

char ready[] = "ready";

void perform(perform_ctxt_t ctxt) {
  ctxt->pself = getpid();
  int p_run[2];
  ASSERT(pipe(p_run) == 0, "pipe failed");

  register_builtin_hook(ctxt->hctxt);
  run_hook_chain(ctxt->hctxt->before_fork, ctxt);
  fflush(log_fp); // avoid multi logging

  // fork child process
  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.\n");

  if (child_pid == 0) { // child process
    ctxt->pchild = getpid();
    child_prework(ctxt);
    write(p_run[1], ready, sizeof(ready));
    child_run(ctxt);
    exit(1); // child process doesn't terminate
  }
  // parent process
  ctxt->pchild = child_pid;

  // wait until child send "ready"
  char receive[6];
  read(p_run[0], receive, sizeof(ready));

  LOG_INFO("parent (%d) child (%d)\n", ctxt->pself, ctxt->pchild);
  run_hook_chain(ctxt->hctxt->after_fork, ctxt);

  int status;
  if (waitpid(child_pid, &status, WUNTRACED) == -1) {
    kill(child_pid, SIGKILL);
    ERRNO_EXIT(-1, "wait failed.\n");
  }

  ctxt->status = status;
  run_hook_chain(ctxt->hctxt->after_wait, ctxt);
  LOG_INFO("judge finished.\n");
}