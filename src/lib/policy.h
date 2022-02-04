#ifndef YAOJUDGE_POLICY_H
#define YAOJUDGE_POLICY_H

#include "common.h"

/**
 * @brief Kafel policy context
 */
struct policy_ctxt {
  char *dirname; // provide policy search path
  char *policy;  // provide without suffix or path, just name, indicating
                 // [dirname]/[policy].policy
  char *content; // string content of the policy
  // policy applied on child process. It may contain some '%s' indentifier used
  // to inject some content during runtime.
  struct sock_fprog prog;
};
struct policy_ctxt create_policy_ctxt(char *dirname, char *policy);

void compile_policy_before_fork(struct perform_ctxt *per_ctxt);

void apply_policy(struct sock_fprog prog);

#endif