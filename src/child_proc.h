/**
 * @file child_proc.h
 * @author sshwy (jy.cat@qq.com)
 * @brief child_proc acts like a layer between tackle (perform) and runner. It
 * describes what to do before runner's prework and after runner's prework. When
 * things are done, you should explictly call prework/run.
 * @date 2022-02-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef YAOJUDGE_CHILD_PROC_H
#define YAOJUDGE_CHILD_PROC_H

#include <sys/types.h>

#include "common.h"

/**
 * preparation (may be heavy work) before run child
 * @param ctxt
 */
void child_prework(perform_ctxt_t ctxt);
void child_run(perform_ctxt_t ctxt);

#endif