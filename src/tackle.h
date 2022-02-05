#ifndef YAOJUDGER_PERFORM_H
#define YAOJUDGER_PERFORM_H

/**
 * @brief Context for tackle, basically hooks.
 *
 */
struct tackle_ctxt {
  // (parent) stuffs before forking child process
  void (*before_fork)(pid_t self);
  // (child) expensive stuffs of child process
  void (*child_prework)(pid_t self);
  // (child) stuffs of child process
  void (*child_run)(pid_t self);
  // (parent) stuffs running immediately after forking child process
  void (*after_fork)(pid_t self, pid_t child);
  // (parent) stuffs after child process terminated.
  void (*after_wait)(pid_t self, pid_t child, int status);
};

typedef struct tackle_ctxt *tackle_ctxt_t;

/**
 * @brief Handle all things under a fork workflow. Basically `tackle` deal with
 * only one thing: fork. Anything else is provided in tackle_ctxt as hooks.
 *
 * @param ctxt
 */
void tackle(tackle_ctxt_t ctxt);

extern const char tk_ready[6];

#endif