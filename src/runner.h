/**
 * @file runner.h
 * @author sshwy (jy.cat@qq.com)
 * @brief General runner programming interface.
 * @date 2022-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef YAOJUDGER_RUNNER_H
#define YAOJUDGER_RUNNER_H

#include "common.h"

/**
 * @brief Context for all runners.
 */
struct runner_ctxt {
  int argc;
  char **argv, **env;
};

/**
 * @brief do some preparation (eg. arguments checking) before applying syscall
 * policy and resource limitation. It should be simple because it's counted in
 * program running time.
 *
 * @param ctxt
 */
void prework(const struct runner_ctxt *ctxt);
void run(const struct runner_ctxt *ctxt);

#endif