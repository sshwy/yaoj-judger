/**
 * @file tkill.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Hooks to open a new thread for timeout killer.
 * @date 2022-02-15
 *
 * @copyright (c) 2022
 *
 */
#ifndef YAOJUDGE_TIMEOUT_KILL_H
#define YAOJUDGE_TIMEOUT_KILL_H

#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "common.h"

int start_killer_after_fork(yjudger_ctxt_t ctxt);
int stop_killer_after_wait(yjudger_ctxt_t ctxt);

#endif