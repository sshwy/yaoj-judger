/**
 * @file traditional.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger for OI's traditional problem.
 * @date 2022-02-05
 * @copyright Copyright (c) 2022
 *
 * It accepts 5 arguments (in
 * runner): the executable filename, input filename, output filename,
 * filename for error output, and finally a token ("std" or "file") determining
 * whether it's standard IO or file IO.
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

int runner_prework(const struct runner_ctxt *ctxt) {
  if (ctxt->argc != 5) {
    SET_ERRORF("invalid arguments (argc=%d expect 5)\n", ctxt->argc);
    return 1;
  }

  if (strcmp(ctxt->argv[4], "std") == 0) {
    const int input_fd = open(ctxt->argv[1], O_RDONLY),
              output_fd = open(ctxt->argv[2], O_WRONLY | O_TRUNC);

    if (input_fd < 0 || output_fd < 0) {
      SET_ERRORF("open \"%s\" or \"%s\" failed", ctxt->argv[1], ctxt->argv[2]);
      return 1;
    }

    if (dup2(input_fd, fileno(stdin)) < 0 ||
        dup2(output_fd, fileno(stdout)) < 0) {
      SET_ERRORF("dup2 failed");
      return 1;
    }
  } else {
    if (strcmp(ctxt->argv[4], "file") != 0) {
      SET_ERRORF("invalid token %s", ctxt->argv[4]);
      return 1;
    }
  }

  // whether is standard IO or file IO, we redirect its stderr stream.
  const int error_fd = open(ctxt->argv[3], O_WRONLY | O_TRUNC);
  if (error_fd < 0) {
    SET_ERRORF("open fd failed");
    return 1;
  }
  if (dup2(error_fd, fileno(stderr)) < 0) {
    SET_ERRORF("dup2 failed");
    return 1;
  }
  return 0;
}

void runner_run(const struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}

int child_prework(perform_ctxt_t ctxt) {
  LOG_INFO("perform child (%d)", ctxt->pchild);
  // sleep(1); // simulate heavy work
  if (runner_prework(ctxt->ectxt))
    return 1;
  if (apply_resource_limit(ctxt))
    return 1;
  if (apply_policy(ctxt))
    return 1;
  fflush(log_fp);
  return 0;
}

void child_run(perform_ctxt_t ctxt) { runner_run(ctxt->ectxt); }

void perform(perform_ctxt_t ctxt) {
  const char ready[] = "ready";
  const char notready[] = "not";

  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  ASSERT(pipe(p_run) == 0, "pipe failed");

  register_hook(ctxt->hctxt, BEFORE_FORK, check_runner_duplicate_before_fork);
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
  LOG_INFO("judge finished.");
}