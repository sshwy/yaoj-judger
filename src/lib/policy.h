#ifndef YAOJUDGE_POLICY_H
#define YAOJUDGE_POLICY_H

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
struct policy_ctxt create_policy_ctxt(char *dirname, char *policy);

void compile_policy_before_fork(perform_ctxt_t per_ctxt);

void apply_policy(perform_ctxt_t per_ctxt);

#endif