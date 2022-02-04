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
#include "lib/policy.h"
#include "runner.h"

/**
 * @brief Perform a runner parameterized by `ectxt`, limited by `pctxt` and
 * `rctxt`
 */
void perform(struct perform_ctxt *ctxt);

#endif /* KAFEL_H */