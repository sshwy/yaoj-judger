#ifndef YAOJUDGE_RESOUCE_LIMIT_H
#define YAOJUDGE_RESOUCE_LIMIT_H

#include "common.h"

/**
 * @brief Resource limitation context.
 */
struct rsclim_ctxt {
  int time;           // in milliseconds. 0 for unlimited
  int virtual_memory; // in bytes, for RLIMIT_AS. 0 for unlimited
  int actual_memory;  // in bytes, compared with ru_maxrss. 0 for unlimited
  int stack_memory;   // in bptes. 0 for unlimited
  int output_size;    // in bytes. 0 for unlimited
};

void apply_resource_limit(perform_ctxt_t ctxt);

void get_usage_after_wait(perform_ctxt_t ctxt);

#endif