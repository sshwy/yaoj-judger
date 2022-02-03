/**
 * @file judger.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger interface. This should be the entry header file, which any
 * other header file should not include.
 * @date 2022-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef YAOJUDGER_H
#define YAOJUDGER_H

#include <linux/filter.h>

#include "common.h"
#include "hook.h"
#include "policy.h"
#include "runner.h"

/**
 * @brief Resource limitation context.
 *
 */
struct rsclim_ctxt {
  int time;           // in milliseconds. 0 for unlimited
  int virtual_memory; // in bytes, for RLIMIT_AS. 0 for unlimited
  int actual_memory;  // in bytes, compared with ru_maxrss. 0 for unlimited
  int stack_memory;   // in bptes. 0 for unlimited
  int output_size;    // in bytes. 0 for unlimited
};

/**
 * @brief Perform a runner parameterized by `ectxt`, limited by `pctxt` and
 * `rctxt`
 */
void perform(struct perform_ctxt *ctxt);

#endif /* KAFEL_H */