/**
 * @file policy.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Kafel policy compilation, enhancement and hooks.
 * @date 2022-02-15
 *
 * @copyright (c) 2022
 *
 */
#ifndef YAOJUDGE_POLICY_H
#define YAOJUDGE_POLICY_H

#include <linux/filter.h>

#include "common.h"

/**
 * @brief Kafel policy context
 */
struct policy_ctxt {
  /// @brief use builtin policy, which means `dirname` should be a tmp folder.
  int is_builtin;
  /// @brief provide policy search path
  char *dirname;
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

struct builtin_policy {
  char *filename;
  unsigned char *content; //!< may not end with NULL
  unsigned int len;
};

typedef struct builtin_policy *builtin_policy_t;
extern const unsigned int policy_num;

struct builtin_policy *policy_list_get();

int policy_set(policy_ctxt_t ctxt, const char *dirname, const char *policy);

int compile_policy_before_fork(yjudger_ctxt_t ctxt);

/**
 * @brief apply policy set in `ctxt` to current process
 * @return 0 on success, 1 otherwise
 */
int apply_policy(yjudger_ctxt_t ctxt);

#endif