/**
 * @file judger.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Judger interface. This should be the entry header file, which any
 * other header file should not include.
 * @date 2022-02-01
 * @copyright Copyright (c) 2022
 *
 * The implementation of judger consists of two parts: common part and perform
 * part. The common part lies in `judger.c` while perform part (i.e., definition
 * of perform()) lies in `src/judger/` with multiple choices. Basically, each
 * definition of perform() is compiled individually and results in
 * corresponding judger library.
 */
#ifndef YAOJUDGER_H
#define YAOJUDGER_H

#include "common.h"

enum rsclim_type {
  REAL_TIME,
  VIR_MEM, //!< shortcut for VIRTUAL_MEMORY
  VIRTUAL_MEMORY,
  ACT_MEM, //!< shortcut for ACTUAL_MEMORY
  ACTUAL_MEMORY,
  STK_MEM, //!< shortcut for STACK_MEMORY
  STACK_MEMORY,
  OUT, //!< shortcut for OUTPUT_SIZE
  OUTPUT_SIZE,
  MEM,    //!< shortcut for MEMORY
  MEMORY, //!< set all three memory limitations
};

/**
 * @brief Context of perform.
 */
struct perform_ctxt {
  /// pid of the current process
  pid_t pself;
  /// pid of the forked child (It's often assumed that the `perform` will do at
  /// least one fork)
  pid_t pchild;

  int status;           //!< child process termination status
  struct rusage rusage; //!< resource usage of child process (getrusage)
  struct result result; //!< perform result of child process

  /// pointer at the policy context (used by builtin_hooks).
  struct policy_ctxt *pctxt;
  /// pointer at the resouce limitation context (used by builtin_hooks).
  struct rsclim_ctxt *rctxt;
  /// pointer at the runner context.
  struct runner_ctxt *ectxt;
  /// pointer at the hook context.
  struct hook_ctxt *hctxt;
};

typedef struct perform_ctxt *perform_ctxt_t;

/**
 * @brief Perform a judgement.
 */
void perform(perform_ctxt_t ctxt);

perform_ctxt_t perform_ctxt_create();

int perform_set_policy(perform_ctxt_t ctxt, char *dirname, char *policy);
int perform_set_runner(perform_ctxt_t ctxt, int argc, char **argv, char **env);
int perform_set_limit(perform_ctxt_t ctxt, int type, int lim);

#endif /* YAOJUDGER_H */