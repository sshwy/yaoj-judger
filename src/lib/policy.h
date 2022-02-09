#ifndef YAOJUDGE_POLICY_H
#define YAOJUDGE_POLICY_H

#include <linux/filter.h>

#include "common.h"

/**
 * @brief Kafel policy context
 */
struct policy_ctxt {
  char *dirname; //!< provide policy search path
  /**
   * @brief Just filename of the policy.
   *
   * provide without suffix or path, just name, indicating
   * [dirname]/[policy].policy
   */
  char *policy;
  /// string content of the policy
  char *content;
  /**
   * @brief  policy applied on child process.
   *
   * It may contain some '%s' indentifier used to inject some content during
   * runtime.
   */
  struct sock_fprog prog;
};

typedef struct policy_ctxt *policy_ctxt_t;

struct policy_ctxt create_policy_ctxt(char *dirname, char *policy);

int compile_policy_before_fork(perform_ctxt_t per_ctxt);

/**
 * @return 0 on success, 1 otherwise
 */
int apply_policy(perform_ctxt_t per_ctxt);

#endif