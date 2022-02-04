#ifndef YAOJUDGE_CHILD_PROC_H
#define YAOJUDGE_CHILD_PROC_H

#include <sys/types.h>

#include "common.h"

/**
 * @brief child_proc acts like a layer between tackle (perform) and runner. It
 * describes what to do before runner's prework and after runner's prework. When
 * things are done, you should explictly call prework/run.
 *
 * @param ctxt
 */
void child_proc(perform_ctxt_t ctxt);

#endif