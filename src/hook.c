#include "hook.h"
#include "common.h"
#include <stdlib.h>

void run_hook_chain(struct hook_chain *phead, struct perform_ctxt *ctxt) {
  fprintf(stderr, "run_hook_chain\n");
  while (phead != NULL) {
    fprintf(stderr, "gg run_hook_chain\n");
    phead->hook(ctxt);
    phead = phead->next;
  }
}
struct hook_chain *create_hook_chain() {
  return NULL;
}
struct hook_chain *pushfront_hook(struct hook_chain *phead,
                                  void (*hook)(struct perform_ctxt *)) {
  struct hook_chain *phook = malloc(sizeof(struct hook_chain));
  phook->hook = hook;
  phook->next = phead;
  return phook;
}
struct hook_ctxt create_hook_ctxt() {
  struct hook_ctxt ctxt = {
      .after_fork = create_hook_chain(),
      .before_fork = create_hook_chain(),
      .after_wait = create_hook_chain(),
      .before_return = create_hook_chain(),
  };
  return ctxt;
}

void register_hook(struct hook_ctxt *ctxt, enum HOOK_TYPE type,
                   void (*hook)(struct perform_ctxt *)) {
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
  case BEFORE_RETURN:
    WORK(before_return);
    break;
  default:
    ASSERT(0, "unknown hook type (%d)", type);
  }
#undef WORK
}