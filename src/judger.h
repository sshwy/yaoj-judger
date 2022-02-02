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
#include "runner/runner.h"

/**
 * @brief Context of perform.
 */
struct perform_ctxt {
  struct sock_fprog
      prog; // policy applied on child process. It may contain some '%s'
            // indentifier used to inject some content during runtime.

  int status; // child prcess status
  struct result result;
  struct rusage rusage;

  // read-only
  const struct policy_ctxt *pctxt;
  const struct rsclim_ctxt *rctxt;
  const struct runner_ctxt *ectxt;
  const struct hook_ctxt *hctxt;
};

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
void perform(struct perform_ctxt *ctxt, struct policy_ctxt pctxt,
             struct rsclim_ctxt rctxt, struct runner_ctxt ectxt,
             struct hook_ctxt hctxt);

#endif /* KAFEL_H */