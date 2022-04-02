/**
 * @file interactive.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger for OI's interactive problem.
 * @date 2022-02-05
 * @copyright Copyright (c) 2022
 *
 * It accepts 6 arguments: the executable filename, ther interactor's filename,
 * interactor's input filename (which the interactor shall use file IO to read),
 * interactor's output filename (use file IO to write), interactor's error
 * output filename, and executable's error output filename. Note that stdin and
 * stdout of interactor and executable will be piped together in a two way
 * communication.
 *
 * Generally if executable occurs some error and stops in advance, the judgement
 * will result in TLE, due to waiting for executable to exchange data through
 * pipes.
 */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "hook.h"
#include "judger.h"
#include "lib/builtin_hook.h"
#include "lib/policy.h"
#include "lib/resource.h"

static int interactor_prework(struct runner_ctxt *ctxt) {
  const int error_fd = open(ctxt->argv[4], O_WRONLY | O_TRUNC);
  if (error_fd < 0)
    yreturn(E_ERRFD);
  if (dup2(error_fd, fileno(stderr)) < 0)
    yreturn(E_DUP);
  return 0;
}

static void run_interactor(struct runner_ctxt *ctxt) {
  execl(ctxt->argv[1], "interactor", ctxt->argv[2], ctxt->argv[3],
        (char *)NULL);
}

static int executable_prework(struct runner_ctxt *ctxt) {
  const int error_fd = open(ctxt->argv[5], O_WRONLY | O_TRUNC);
  if (error_fd < 0)
    yreturn(E_ERRFD);
  if (dup2(error_fd, fileno(stderr)) < 0)
    yreturn(E_DUP);
  return 0;
}

static void run_executable(struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}

static const char ready[] = "ready";
static const char notready[] = "not";

static void yexit_notready(int code, int write_pipe) {
  write(write_pipe, notready, sizeof(notready));
  yexit(code);
}

static void child_exec_process(yjudger_ctxt_t ctxt, int i_run[2]) {
  if (getpgrp() != ctxt->pchild) {
    yexit_notready(E_PGID, i_run[1]);
  }
  if (executable_prework(ctxt->ectxt) || apply_resource_limit(ctxt) ||
      apply_policy(ctxt)) {
    yexit_notready(yerrno, i_run[1]);
  }
  LOG_INFO("executable prepared");
  fflush(log_fp);
  // assume that exec* doesn't change pgid
  // https://www.cs.uleth.ca/~holzmann/C/system/pipeforkexec.html
  // use chmod could reset the suid/sgid mode
  write(i_run[1], ready, sizeof(ready));
  run_executable(ctxt->ectxt);
  exit(E_EXEC); // process doesn't terminate
}

static void child_process(yjudger_ctxt_t ctxt, int *p_run) { // child process
  ctxt->pchild = getpid();
  int itoe[2], etoi[2], i_run[2];
  // set process group ID to itself
  if (setpgid(0, 0)) {
    yexit_notready(E_SETPGID, p_run[1]);
  }
  if (pipe(itoe) || pipe(etoi) || pipe(i_run)) {
    yexit_notready(E_PIPE, p_run[1]);
  }

  fflush(log_fp); // avoid multi logging
  // when a process is forked, it inherits its PGID from its parent.
  // https://unix.stackexchange.com/questions/139222/why-is-the-pgid-of-my-child-processes-not-the-pid-of-the-parent
  const pid_t exec_pid = fork(); // fork again
  if (exec_pid == 0) {           // process for executable
    if (dup2(itoe[0], fileno(stdin)) == -1 ||
        dup2(etoi[1], fileno(stdout)) == -1) {
      yexit_notready(E_DUP, i_run[1]);
    }
    child_exec_process(ctxt, i_run);
  }
  // process for interactor
  if (dup2(etoi[0], fileno(stdin)) == -1 ||
      dup2(itoe[1], fileno(stdout)) == -1) {
    yexit_notready(E_DUP, p_run[1]);
  }
  // apply the same policy for interactor
  if (interactor_prework(ctxt->ectxt) || apply_policy(ctxt)) {
    yexit_notready(yerrno, p_run[1]);
  }
  LOG_INFO("interactor prepared");
  // when executable is ready, tell judger that I'm ready
  char receive[6];
  if (read(i_run[0], receive, sizeof(ready)) != sizeof(ready)) {
    yexit_notready(E_INTACT_EXEC, p_run[1]);
  }
  write(p_run[1], ready, sizeof(ready));

  fflush(log_fp);
  run_interactor(ctxt->ectxt);
  exit(E_EXEC); // process doesn't terminate
}

int yjudger_interactive(yjudger_ctxt_t ctxt) {
  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  if (pipe(p_run)) {
    yreturn(E_PIPE);
  }
  if (ctxt->ectxt->argc != 6) {
    yreturn(E_ARGC);
  }

  register_hook(ctxt->hctxt, BEFORE_FORK, check_runner_duplicate_before_fork);
  register_builtin_hook(ctxt->hctxt);
  if (run_hook_chain(ctxt->hctxt->before_fork, ctxt))
    yreturn(yerrno);
  fflush(log_fp); // avoid multi logging

  // fork child process
  const pid_t child_pid = fork();
  if (child_pid < 0) {
    yreturn(E_FORK);
  }

  if (child_pid == 0) {
    child_process(ctxt, p_run);
  }
  // parent process
  LOG_DEBUG("after fork");
  ctxt->pchild = child_pid;

  // wait until child send "ready"
  char receive[6];
  if (read(p_run[0], receive, sizeof(ready)) != sizeof(ready)) {
    yreturn(E_CHILD);
  }

  LOG_INFO("parent (%d) child (%d)", ctxt->pself, ctxt->pchild);
  if (run_hook_chain(ctxt->hctxt->after_fork, ctxt)) {
    yreturn(yerrno);
  }

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