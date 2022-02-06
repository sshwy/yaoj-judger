/**
 * @file hook.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Hooks used by judger.
 * @date 2022-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef YAOJUDGER_HOOK_H
#define YAOJUDGER_HOOK_H

#include "common.h"

enum HOOK_TYPE {
  BEFORE_FORK,
  AFTER_FORK,
  AFTER_WAIT,
};

typedef void (*hook_func_t)(perform_ctxt_t);

/**
 * @brief Link list storing hook functions. Note that it's a typical FILO list
 * so the ealier you register a hook, the later it will be invoked.
 */
struct hook_chain {
  hook_func_t hook;
  struct hook_chain *next;
};
typedef struct hook_chain *hook_chain_t;

/**
 * @brief Context for custom hooks
 */
struct hook_ctxt {
  /// invoked before forking the child process.
  hook_chain_t before_fork;
  /// invoked right after forking the child process.
  hook_chain_t after_fork;
  /// invoked right after child process's termination.
  hook_chain_t after_wait;
};

/**
 * @brief Run all hooks one by one in the list with context specified.
 * @param phead
 * @param rlp
 */
void run_hook_chain(hook_chain_t phead, perform_ctxt_t ctxt);

/**
 * @brief Create an empty hook chain, returning its head pointer.
 * @return struct hook_chain*
 */
hook_chain_t create_hook_chain();

hook_chain_t pushfront_hook(hook_chain_t phead, hook_func_t hook);

void register_hook(struct hook_ctxt *ctxt, enum HOOK_TYPE type,
                   hook_func_t hook);

struct hook_ctxt create_hook_ctxt();
#endif