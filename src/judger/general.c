/**
 * @file general.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger for general commands.
 * @date 2022-02-08
 * @copyright Copyright (c) 2022
 *
 * For runner arguments: argv is an array of pointers to strings, the first of
 * which (ie., `argv[0]`) acts like `pathname` in execve(), and the rest of
 * which are passed to the new program as its command-line arguments. By
 * convention, `argv[1]` should contain the filename associated with the file
 * being executed. The argv array must be terminated by a NULL pointer. (Thus,
 * in the new program, argv[argc] will be NULL.)
 *
 * envp is an array of pointers to strings, conventionally of the form
 * `key=value`, which are passed as the environment of the new program.  The
 * envp array must be terminated by a NULL pointer.
 */
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "hook.h"
#include "judger.h"
#include "lib/builtin_hook.h"
#include "lib/policy.h"
#include "lib/resource.h"

void runner_run(struct runner_ctxt *ctxt) {
  execve(ctxt->argv[0], ctxt->argv + 1, ctxt->env);
}
void child_run(perform_ctxt_t ctxt) { runner_run(ctxt->ectxt); }

/**
 * @return 0 on success, 1 otherwise
 */
int child_prework(perform_ctxt_t ctxt) {
  LOG_INFO("perform child (%d)", ctxt->pchild);
  if (apply_resource_limit(ctxt))
    return 1;
  if (apply_policy(ctxt))
    return 1;
  fflush(log_fp);
  return 0;
}

void perform(perform_ctxt_t ctxt) {
  const char ready[] = "ready";
  const char notready[] = "not";

  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  ASSERT(pipe(p_run) == 0, "pipe failed");

  register_builtin_hook(ctxt->hctxt);
  if (run_hook_chain(ctxt->hctxt->before_fork, ctxt))
    EXIT_WITHMSG();
  fflush(log_fp); // avoid multi logging

  // fork child process
  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.\n");

  if (child_pid == 0) { // child process
    ctxt->pchild = getpid();
    if (setpgid(0, 0)) { // set process group ID to itself
      SET_ERRORF("setpgid failed");
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }
    if (child_prework(ctxt)) {
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }
    write(p_run[1], ready, sizeof(ready));
    child_run(ctxt);
    exit(1); // child process doesn't terminate
  }
  // parent process
  ctxt->pchild = child_pid;

  // wait until child send "ready"
  char receive[6];
  if (read(p_run[0], receive, sizeof(ready)) != sizeof(ready)) {
    SET_ERRORF("child process not ready");
    EXIT_WITHMSG();
  }

  LOG_INFO("parent (%d) child (%d)", ctxt->pself, ctxt->pchild);
  if (run_hook_chain(ctxt->hctxt->after_fork, ctxt))
    EXIT_WITHMSG();

  int status;
  if (waitpid(child_pid, &status, WUNTRACED) == -1) {
    killpg(child_pid, SIGKILL);
    SET_ERRORF("wait failed");
    EXIT_WITHMSG();
  }

  ctxt->status = status;
  if (run_hook_chain(ctxt->hctxt->after_wait, ctxt)) {
    EXIT_WITHMSG();
  }
  LOG_INFO("judge finished");
}