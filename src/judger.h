/**
 * @file judger.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger interface.
 * @date 2022-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef YAOJUDGER_H
#define YAOJUDGER_H

#include <linux/filter.h>

#include "./common.h"
#include "hook.h"

/**
 * @brief Perform a runner parameterized by `ectxt`, limited by `pctxt` and
 * `rctxt`
 */
void perform(struct perform_ctxt *ctxt, struct policy_ctxt pctxt,
             struct rsclim_ctxt rctxt, struct runner_ctxt ectxt,
             struct hook_ctxt hctxt);

#endif /* KAFEL_H */