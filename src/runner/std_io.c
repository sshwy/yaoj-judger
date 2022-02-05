/**
 * @file std_io.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Runner for standard IO program.
 * @date 2022-02-01
 * @copyright Copyright (c) 2022
 *
 * arguments: [exec] [input] [output] [error]
 *   [exec] for executable filename
 *   [input] for input filename
 *   [output] for output filename
 *   [error] for error log filename
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "runner.h"
// std_io: exec input_file output_file error_file

void runner_prework(const struct runner_ctxt *ctxt) {
  ASSERT(ctxt->argc == 4, "invalid arguments (argc=%d expect 4)\n", ctxt->argc);

  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      ASSERT(strcmp(ctxt->argv[i], ctxt->argv[j]) != 0,
             "duplicated files! (i=%d, j=%d", i, j);
    }
  }

  const int input_fd = open(ctxt->argv[1], O_RDONLY),
            output_fd = open(ctxt->argv[2], O_WRONLY | O_TRUNC),
            error_fd = open(ctxt->argv[3], O_WRONLY | O_TRUNC);

  if (input_fd < 0 || output_fd < 0 || error_fd < 0) {
    ERRNO_EXIT(errno, "open file descriptor failed.\n");
  }

  if (dup2(input_fd, fileno(stdin)) < 0) {
    ERRNO_EXIT(errno, "refer stdin to input_fd failed.\n");
  }
  if (dup2(output_fd, fileno(stdout)) < 0) {
    ERRNO_EXIT(errno, "refer stdout to output_fd failed.\n");
  }
  if (dup2(error_fd, fileno(stderr)) < 0) {
    ERRNO_EXIT(errno, "refer stdout to output_fd failed.\n");
  }
}

void runner_run(const struct runner_ctxt *ctxt) {
  execl(ctxt->argv[0], "main", (char *)NULL);
}