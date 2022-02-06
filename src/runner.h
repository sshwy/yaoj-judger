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

#endif