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

#include "../common.h"

/**
 * return EXIT_SUCCESS (0) if pass.
 */
void prework(const struct runner_ctxt *ctxt);
void run(const struct runner_ctxt *ctxt);

#endif