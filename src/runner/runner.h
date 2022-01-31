#ifndef YAOJUDGER_RUNNER_H
#define YAOJUDGER_RUNNER_H

#include "../common.h"

/**
 * return EXIT_SUCCESS (0) if pass.
 */
int prework(const struct runner_ctxt ctxt);
int run(const struct runner_ctxt ctxt);

#endif