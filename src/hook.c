#include <stdlib.h>

#include "common.h"
#include "hook.h"

void run_hook_chain(hook_chain_t phead, perform_ctxt_t ctxt) {
  while (phead != NULL) {
    phead->hook(ctxt);
    phead = phead->next;
  }
}
hook_chain_t create_hook_chain() { return NULL; }
hook_chain_t pushfront_hook(hook_chain_t phead, hook_func_t hook) {
  hook_chain_t phook = malloc(sizeof(struct hook_chain));
  phook->hook = hook;
  phook->next = phead;
  return phook;
}
struct hook_ctxt create_hook_ctxt() {
  struct hook_ctxt ctxt = {
      .after_fork = create_hook_chain(),
      .before_fork = create_hook_chain(),
      .after_wait = create_hook_chain(),
  };
  return ctxt;
}

void register_hook(struct hook_ctxt *ctxt, enum HOOK_TYPE type,
                   hook_func_t hook) {
#define WORK(name) ctxt->name = pushfront_hook(ctxt->name, hook)
  switch (type) {
  case BEFORE_FORK:
    WORK(before_fork);
    break;
  case AFTER_FORK:
    WORK(after_fork);
    break;
  case AFTER_WAIT:
    WORK(after_wait);
    break;
  default:
    ASSERT(0, "unknown hook type (%d)", type);
  }
#undef WORK
}