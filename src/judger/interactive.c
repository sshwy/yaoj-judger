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
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "hook.h"
#include "judger.h"
#include "lib/builtin_hook.h"
#include "lib/policy.h"
#include "lib/resouce.h"
#include "runner.h"

char ready[] = "ready";

static void interactor_prework(struct runner_ctxt *ctxt) {
  const int error_fd = open(ctxt->argv[4], O_WRONLY | O_TRUNC);
  ASSERT(error_fd >= 0, "open fd failed.\n");
  ASSERT(dup2(error_fd, fileno(stderr)) >= 0, "dup2 failed.\n");
}

static void run_interactor(struct runner_ctxt *ctxt) {
  execl(ctxt->argv[1], "interactor", (char *)NULL);
}

static void executable_prework(struct runner_ctxt *ctxt) {
  const int error_fd = open(ctxt->argv[5], O_WRONLY | O_TRUNC);
  ASSERT(error_fd >= 0, "open fd failed.\n");
  ASSERT(dup2(error_fd, fileno(stderr)) >= 0, "dup2 failed.\n");
}

static void run_executable(struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}

void perform(perform_ctxt_t ctxt) {
  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  ASSERT(pipe(p_run) == 0, "pipe failed");
  ASSERT(ctxt->ectxt->argc == 6, "invalid arguments (argc=%d expect 6)\n",
         ctxt->ectxt->argc);

  register_builtin_hook(ctxt->hctxt);
  run_hook_chain(ctxt->hctxt->before_fork, ctxt);
  fflush(log_fp); // avoid multi logging

  // fork child process
  const pid_t child_pid = fork();
  ASSERT(child_pid >= 0, "fork failed.\n");

  if (child_pid == 0) { // child process
    ctxt->pchild = getpid();
    // set process group ID to itself
    ASSERT(setpgid(0, 0) == 0, "setpgid failed");
    int itoe[2], etoi[2], i_run[2];
    ASSERT(pipe(itoe) == 0 && pipe(etoi) == 0, "create pipe failed");
    ASSERT(pipe(i_run) == 0, "create pipe failed");

    fflush(log_fp); // avoid multi logging
    // when a process is forked, it inherits its PGID from its parent.
    // https://unix.stackexchange.com/questions/139222/why-is-the-pgid-of-my-child-processes-not-the-pid-of-the-parent
    const pid_t exec_pid = fork(); // fork again
    if (exec_pid == 0) {           // process for executable
      ASSERT(getpgrp() == ctxt->pchild, "pgid != interactor's pid!\n");
      ASSERT(dup2(itoe[0], fileno(stdin)) != -1, "dup2 failed.\n");
      ASSERT(dup2(etoi[1], fileno(stdout)) != -1, "dup2 failed.\n");
      executable_prework(ctxt->ectxt);
      apply_resource_limit(ctxt);
      apply_policy(ctxt); // apply policy for executable
      LOG_INFO("executable prepared.\n");
      fflush(log_fp);
      // assume that exec* doesn't change pgid
      // https://www.cs.uleth.ca/~holzmann/C/system/pipeforkexec.html
      // use chmod could reset the suid/sgid mode
      write(i_run[1], ready, sizeof(ready));
      run_executable(ctxt->ectxt);
      exit(1); // process doesn't terminate
    }
    // process for interactor
    ASSERT(dup2(etoi[0], fileno(stdin)) != -1, "dup2 failed.\n");
    ASSERT(dup2(itoe[1], fileno(stdout)) != -1, "dup2 failed.\n");
    interactor_prework(ctxt->ectxt);
    apply_policy(ctxt); // apply the same policy for interactor
    LOG_INFO("interactor prepared.\n");
    // when executable is ready, tell judger that I'm ready
    char receive[6];
    read(i_run[0], receive, sizeof(ready));
    write(p_run[1], ready, sizeof(ready));

    fflush(log_fp);
    run_interactor(ctxt->ectxt);
    exit(1); // process doesn't terminate
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
    killpg(child_pid, SIGKILL);
    ERRNO_EXIT(-1, "wait failed.\n");
  }

  ctxt->status = status;
  run_hook_chain(ctxt->hctxt->after_wait, ctxt);
  LOG_INFO("judge finished.\n");
}