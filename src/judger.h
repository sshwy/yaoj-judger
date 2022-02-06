/**
 * @file judger.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger interface. This should be the entry header file, which any
 * other header file should not include.
 * @date 2022-02-01
 * @copyright Copyright (c) 2022
 */
#ifndef YAOJUDGER_H
#define YAOJUDGER_H

#include "common.h"
#include "hook.h"
#include "lib/policy.h"
#include "lib/resouce.h"
#include "runner.h"

/**
 * @brief Perform a judgement.
 */
void perform(perform_ctxt_t ctxt);

#endif /* YAOJUDGER_H */