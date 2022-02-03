#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "tackle.h"

void tackle(tackle_ctxt_t ctxt) {
  const pid_t parent_pid = getpid();

  ctxt->before_fork(parent_pid);

  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.\n");

  if (child_pid == 0) {
    const pid_t real_child_pid = getpid();
    ctxt->child_proc(real_child_pid);
    return;
  }

  ctxt->after_fork(parent_pid, child_pid);

  int status;
  if (waitpid(child_pid, &status, WUNTRACED) == -1) {
    kill(child_pid, SIGKILL);
    ERRNO_EXIT(-1, "wait failed.\n");
  }

  ctxt->after_wait(parent_pid, child_pid, status);
}