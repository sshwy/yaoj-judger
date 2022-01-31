#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "runner.h"
// std_io: exec input_file output_file error_file

int prework(const struct runner_ctxt ctxt) {
  if (ctxt.argc < 4) {
    ERRNO_EXIT(
        EXIT_FAILURE,
        "invalid arguments (argc=%d), expect [exec] [input] [output] [error]\n",
        ctxt.argc);
  }

  for (int i = 0; i < 4; i++) {
    for (int j = i + 1; j < 4; j++) {
      if (strcmp(ctxt.argv[i], ctxt.argv[j]) == 0) {
        ERRNO_EXIT(EXIT_FAILURE,
                   "duplicated files! (exec=%s,input=%s,output=%s,error=%s)\n",
                   ctxt.argv[0], ctxt.argv[1], ctxt.argv[2], ctxt.argv[3]);
      }
    }
  }

  const char *filename = ctxt.argv[0];

  const int input_fd = open(ctxt.argv[1], O_RDONLY),
            output_fd = open(ctxt.argv[2], O_WRONLY | O_TRUNC),
            error_fd = open(ctxt.argv[3], O_WRONLY | O_TRUNC);

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

  return EXIT_SUCCESS;
}

int run(const struct runner_ctxt ctxt) {
  int flag = execl(ctxt.argv[0], (char *)NULL);
  printf("return\n");
  return flag;
}