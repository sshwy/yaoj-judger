/**
 * @file traditional.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger for OI's traditional problem.
 * @date 2022-02-05
 * @copyright Copyright (c) 2022
 *
 * It accepts 5 arguments (in runner): the executable filename, input filename,
 * output filename, filename for error output, and finally a token ("std" or
 * "file") determining whether it's standard IO or file IO.
 */
#define _GNU_SOURCE // https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html

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
#include "lib/resource.h"

static int runner_prework(const struct runner_ctxt *ctxt) {
  if (ctxt->argc != 5) {
    yreturn(E_ARGC);
  }

  if (strcmp(ctxt->argv[4], "std") == 0) {
    const int input_fd = open(ctxt->argv[1], O_RDONLY),
              output_fd = open(ctxt->argv[2], O_WRONLY | O_TRUNC);

    if (input_fd < 0 || output_fd < 0) {
      yreturn(E_FILEFD);
    }

    if (dup2(input_fd, fileno(stdin)) < 0 ||
        dup2(output_fd, fileno(stdout)) < 0) {
      yreturn(E_DUP);
    }
  } else {
    if (strcmp(ctxt->argv[4], "file") != 0) {
      yreturn(E_TOKEN);
    }
  }

  // whether is standard IO or file IO, we redirect its stderr stream.
  const int error_fd = open(ctxt->argv[3], O_WRONLY | O_TRUNC);
  if (error_fd < 0) {
    yreturn(E_ERRFD);
  }
  if (dup2(error_fd, fileno(stderr)) < 0) {
    yreturn(E_DUP);
  }
  return 0;
}

static void runner_run(const struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}

static int child_prework(yjudger_ctxt_t ctxt) {
  LOG_INFO("perform child (%d)", ctxt->pchild);
  // sleep(1); // simulate heavy work
  if (runner_prework(ctxt->ectxt) || apply_resource_limit(ctxt) ||
      apply_policy(ctxt))
    yreturn(yerrno);
  fflush(log_fp);
  return 0;
}

int yjudger_traditional(yjudger_ctxt_t ctxt) {
  const char ready[] = "ready";
  const char notready[] = "not";

  ctxt->pself = getpid();
  ctxt->pchild = -1;
  int p_run[2];
  if (pipe(p_run))
    yreturn(E_PIPE);

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
    runner_run(ctxt->ectxt);
    exit(E_EXEC); // child process doesn't terminate
  }
  // parent process
  ctxt->pchild = child_pid;

  // wait until child send "ready"
  char receive[6];
  if (read(p_run[0], receive, sizeof(ready)) != sizeof(ready)) {
    yreturn(E_CHILD);
  }

  LOG_INFO("parent (%d) child (%d)", ctxt->pself, ctxt->pchild);
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