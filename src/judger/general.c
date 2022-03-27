/**
 * @file general.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger for general commands.
 * @date 2022-02-08
 * @copyright Copyright (c) 2022
 *
 * For runner arguments: argv contains one argument: the executable script file.
 *
 * envp is an array of pointers to strings, conventionally of the form
 * `key=value`, which are passed as the environment of the new program.  The
 * envp array must be terminated by a NULL pointer.
 */
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "hook.h"
#include "judger.h"
#include "lib/builtin_hook.h"
#include "lib/policy.h"
#include "lib/resource.h"

static void runner_run(struct runner_ctxt *ctxt) {
  execve(ctxt->argv[0], ctxt->argv, ctxt->env);
}

/**
 * @return 0 on success, 1 otherwise
 */
static int child_prework(perform_ctxt_t ctxt) {
  LOG_DEBUG("perform child (%d)", ctxt->pchild);
  if (apply_resource_limit(ctxt) || apply_policy(ctxt))
    yreturn(yerrno);
  fflush(log_fp);
  return 0;
}

int perform_general(perform_ctxt_t ctxt) {
  const char ready[] = "ready";
  const char notready[] = "not";

  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  if (pipe(p_run)) {
    yreturn(E_PIPE);
  }

  register_builtin_hook(ctxt->hctxt);
  if (run_hook_chain(ctxt->hctxt->before_fork, ctxt))
    yreturn(yerrno);
  fflush(log_fp); // avoid multi logging

  // fork child process
  const pid_t child_pid = fork();
  if (child_pid < 0) {
    yreturn(E_FORK);
  }

  if (child_pid == 0) { // child process
    ctxt->pchild = getpid();
    if (setpgid(0, 0)) { // set process group ID to itself
      write(p_run[1], notready, sizeof(notready));
      yexit(E_SETPGID);
    }
    if (child_prework(ctxt)) {
      write(p_run[1], notready, sizeof(notready));
      yexit(yerrno);
    }
    write(p_run[1], ready, sizeof(ready));
    fflush(log_fp);

    runner_run(ctxt->ectxt);
    if (errno == EACCES) {
      LOG_ERROR("exec error getting access");
    }
    yexit(E_EXEC); // child process doesn't terminate
  }
  // parent process
  ctxt->pchild = child_pid;

  // wait until child send "ready"
  char receive[6];
  if (read(p_run[0], receive, sizeof(ready)) != sizeof(ready)) {
    yreturn(E_CHILD);
  }

  LOG_DEBUG("parent (%d) child (%d)", ctxt->pself, ctxt->pchild);
  if (run_hook_chain(ctxt->hctxt->after_fork, ctxt))
    yreturn(yerrno);

  int status;
  if (waitpid(child_pid, &status, WUNTRACED) == -1) {
    killpg(child_pid, SIGKILL);
    yreturn(E_WAIT);
  }

  ctxt->status = status;
  if (run_hook_chain(ctxt->hctxt->after_wait, ctxt)) {
    yreturn(yerrno);
  }
  LOG_INFO("judge finished");
  return 0;
}