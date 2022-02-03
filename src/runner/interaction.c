/**
 * @file interaction.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Runner for interactive problems' code.
 * @date 2022-02-03
 * @ref https://notes.sshwy.name/Interactive-Prob-Judger/
 * @copyright Copyright (c) 2022
 *
 */

#include <unistd.h>

#include "runner.h"

void runner_hook(const struct perform_ctxt *ctxt) {}

static int p1[2], p2[2]; // pipe

void prework(const struct runner_ctxt *ctxt) {
  ASSERT(ctxt->argc == 5, "invalid arguments (argc=%d expect 5)\n", ctxt->argc);

  for (int i = 0; i < 5; i++) {
    for (int j = i + 1; j < 5; j++) {
      ASSERT(strcmp(ctxt->argv[i], ctxt->argv[j]) != 0,
             "duplicated files! (i=%d, j=%d", i, j);
    }
  }
  ASSERT(pipe(p1) == 0 && pipe(p2) == 0, "create pipe failed.\n");
}
void run(const struct runner_ctxt *ctxt) {}