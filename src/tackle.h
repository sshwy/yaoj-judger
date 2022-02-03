#ifndef YAOJUDGER_PERFORM_H
#define YAOJUDGER_PERFORM_H

/**
 * @brief Context for tackle, basically hooks.
 *
 */
struct tackle_ctxt {
  void (*before_fork)(pid_t self);
  void (*child_proc)(pid_t self);
  void (*after_fork)(pid_t self, pid_t child);
  void (*after_wait)(pid_t self, pid_t child, int status);
};

typedef struct tackle_ctxt *tackle_ctxt_t;

/**
 * @brief handle all things under a fork workflow
 *
 * @param ctxt
 */
void tackle(tackle_ctxt_t ctxt);

#endif