/**
 * @file builtin_hook.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Part of the builtin hooks
 * @date 2022-02-15
 *
 * @copyright (c) 2022
 *
 */
#ifndef YAOJUDGE_BUILTIN_HOOK_H
#define YAOJUDGE_BUILTIN_HOOK_H

#include "hook.h"

/**
 * @brief Optional builtin hook. Check if runner's arguments has duplicate
 * string.
 *
 * @param ctxt
 * @return int
 */
int check_runner_duplicate_before_fork(yjudger_ctxt_t ctxt);

void register_builtin_hook(hook_ctxt_t hctxt);

#endif