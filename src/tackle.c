#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "tackle.h"

const char tk_ready[6] = "ready";

void tackle(tackle_ctxt_t ctxt) {
  const pid_t parent_pid = getpid();
  int p_run[2];
  ASSERT(pipe(p_run) == 0, "pipe failed");

  ctxt->before_fork(parent_pid);

  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.\n");

  if (child_pid == 0) {
    const pid_t real_child_pid = getpid();
    ctxt->child_prework(real_child_pid);

    write(p_run[1], tk_ready, sizeof(tk_ready));

    ctxt->child_run(real_child_pid);
    exit(1); // child process doesn't terminate
  }

  char receive[6];
  // wait until ready to child_run
  read(p_run[0], receive, sizeof(tk_ready));

  ctxt->after_fork(parent_pid, child_pid);

  int status;
  if (waitpid(child_pid, &status, WUNTRACED) == -1) {
    kill(child_pid, SIGKILL);
    ERRNO_EXIT(-1, "wait failed.\n");
  }

  ctxt->after_wait(parent_pid, child_pid, status);
}