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

struct hook_chain {
  void (*hook)(perform_ctxt_t ctxt);
  struct hook_chain *next;
};

/**
 * @brief Context for custom hooks
 *
 */
struct hook_ctxt {
  // invoked before forking the child process
  struct hook_chain *before_fork;
  // invoked right after forking the child process
  struct hook_chain *after_fork;
  // invoked right after child process's termination
  struct hook_chain *after_wait;
};

/**
 * @brief Run all hooks one by one in the list with param `rlp`
 * @param phead
 * @param rlp
 */
void run_hook_chain(struct hook_chain *phead, perform_ctxt_t ctxt);

/**
 * @brief Create an empty hook chain, returning its head pointer.
 * @return struct hook_chain*
 */
struct hook_chain *create_hook_chain();

struct hook_chain *pushfront_hook(struct hook_chain *phead,
                                  void (*hook)(perform_ctxt_t));

void register_hook(struct hook_ctxt *ctxt, enum HOOK_TYPE type,
                   void (*hook)(perform_ctxt_t ctxt));

struct hook_ctxt create_hook_ctxt();
#endif