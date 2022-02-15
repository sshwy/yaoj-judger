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
  if (error_fd < 0) {
    SET_ERRORF("open fd (%s) failed", ctxt->argv[4]);
    return 1;
  }
  if (dup2(error_fd, fileno(stderr)) < 0) {
    SET_ERRORF("dup2 failed");
    return 1;
  }
  return 0;
}

static void run_interactor(struct runner_ctxt *ctxt) {
  execl(ctxt->argv[1], "interactor", (char *)NULL);
}

static int executable_prework(struct runner_ctxt *ctxt) {
  const int error_fd = open(ctxt->argv[5], O_WRONLY | O_TRUNC);
  if (error_fd < 0) {
    SET_ERRORF("open fd (%s) failed", ctxt->argv[5]);
    return 1;
  }
  if (dup2(error_fd, fileno(stderr)) < 0) {
    SET_ERRORF("dup2 failed");
    return 1;
  }
  return 0;
}

static void run_executable(struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}

void perform(perform_ctxt_t ctxt) {
  const char ready[] = "ready";
  const char notready[] = "not";

  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  if (pipe(p_run)) {
    SET_ERRORF("pipe failed");
    EXIT_WITHMSG();
  }
  if (ctxt->ectxt->argc != 6) {
    SET_ERRORF("invalid arguments (argc=%d expect 6)", ctxt->ectxt->argc);
    EXIT_WITHMSG();
  }

  register_hook(ctxt->hctxt, BEFORE_FORK, check_runner_duplicate_before_fork);
  register_builtin_hook(ctxt->hctxt);
  if (run_hook_chain(ctxt->hctxt->before_fork, ctxt))
    EXIT_WITHMSG();
  fflush(log_fp); // avoid multi logging

  // fork child process
  const pid_t child_pid = fork();
  if (child_pid < 0) {
    SET_ERRORF("fork failed");
    EXIT_WITHMSG();
  }

  if (child_pid == 0) { // child process
    ctxt->pchild = getpid();
    int itoe[2], etoi[2], i_run[2];
    // set process group ID to itself
    if (setpgid(0, 0)) {
      SET_ERRORF("setpgid failed");
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }
    if (pipe(itoe) || pipe(etoi) || pipe(i_run)) {
      SET_ERRORF("create pipe failed");
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }

    fflush(log_fp); // avoid multi logging
    // when a process is forked, it inherits its PGID from its parent.
    // https://unix.stackexchange.com/questions/139222/why-is-the-pgid-of-my-child-processes-not-the-pid-of-the-parent
    const pid_t exec_pid = fork(); // fork again
    if (exec_pid == 0) {           // process for executable
      if (getpgrp() != ctxt->pchild) {
        SET_ERRORF("pgid != interactor's pid!");
        write(i_run[1], notready, sizeof(notready));
        EXIT_WITHMSG();
      }
      if (dup2(itoe[0], fileno(stdin)) == -1 ||
          dup2(etoi[1], fileno(stdout)) == -1) {
        SET_ERRORF("dup2 failed");
        write(i_run[1], notready, sizeof(notready));
        EXIT_WITHMSG();
      }
      if (executable_prework(ctxt->ectxt) || apply_resource_limit(ctxt) ||
          apply_policy(ctxt)) {
        write(i_run[1], notready, sizeof(notready));
        EXIT_WITHMSG();
      }
      LOG_INFO("executable prepared");
      fflush(log_fp);
      // assume that exec* doesn't change pgid
      // https://www.cs.uleth.ca/~holzmann/C/system/pipeforkexec.html
      // use chmod could reset the suid/sgid mode
      write(i_run[1], ready, sizeof(ready));
      run_executable(ctxt->ectxt);
      exit(1); // process doesn't terminate
    }
    // process for interactor
    if (dup2(etoi[0], fileno(stdin)) == -1 ||
        dup2(itoe[1], fileno(stdout)) == -1) {
      SET_ERRORF("dup2 failed");
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }
    // apply the same policy for interactor
    if (interactor_prework(ctxt->ectxt) || apply_policy(ctxt)) {
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }
    LOG_INFO("interactor prepared");
    // when executable is ready, tell judger that I'm ready
    char receive[6];
    if (read(i_run[0], receive, sizeof(ready)) != sizeof(ready)) {
      SET_ERRORF("exec not ready");
      write(p_run[1], notready, sizeof(notready));
      EXIT_WITHMSG();
    }
    write(p_run[1], ready, sizeof(ready));

    fflush(log_fp);
    run_interactor(ctxt->ectxt);
    exit(1); // process doesn't terminate
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
  if (run_hook_chain(ctxt->hctxt->after_fork, ctxt)) {
    EXIT_WITHMSG();
  }

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