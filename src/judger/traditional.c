/**
 * @file traditional.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger for OI's traditional problem. It accepts 5 arguments (in
 * runner): the executable filename, input filename, output filename,
 * filename for error output, and finally a token ("std" or "file") determining
 * whether it's standard IO or file IO.
 * @date 2022-02-05
 *
 * @copyright Copyright (c) 2022
 *
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

void runner_prework(const struct runner_ctxt *ctxt) {
  ASSERT(ctxt->argc == 5, "invalid arguments (argc=%d expect 5)\n", ctxt->argc);

  if (strcmp(ctxt->argv[4], "std") == 0) {
    const int input_fd = open(ctxt->argv[1], O_RDONLY),
              output_fd = open(ctxt->argv[2], O_WRONLY | O_TRUNC);

    if (input_fd < 0 || output_fd < 0) {
      ERRNO_EXIT(errno, "open fd failed.\n");
    }

    if (dup2(input_fd, fileno(stdin)) < 0) {
      ERRNO_EXIT(errno, "refer stdin to input_fd failed.\n");
    }
    if (dup2(output_fd, fileno(stdout)) < 0) {
      ERRNO_EXIT(errno, "refer stdout to output_fd failed.\n");
    }
  } else {
    ASSERT(strcmp(ctxt->argv[4], "file") == 0, "invalid token %s",
           ctxt->argv[4]);
  }

  // whether is standard IO or file IO, we redirect its stderr stream.
  const int error_fd = open(ctxt->argv[3], O_WRONLY | O_TRUNC);
  ASSERT(error_fd >= 0, "open fd failed.\n");

  if (dup2(error_fd, fileno(stderr)) < 0) {
    ERRNO_EXIT(errno, "refer stdout to output_fd failed.\n");
  }
}

void runner_run(const struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}

void child_prework(perform_ctxt_t ctxt) {
  LOG_INFO("perform child (%d)\n", ctxt->pchild);
  // sleep(1); // simulate heavy work
  runner_prework(ctxt->ectxt);
  apply_resource_limit(ctxt);
  apply_policy(ctxt);
  fflush(log_fp);
  fclose(log_fp); // !!!important
}

void child_run(perform_ctxt_t ctxt) { runner_run(ctxt->ectxt); }

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