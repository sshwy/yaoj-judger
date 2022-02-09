#ifndef YAOJUDGE_RESOUCE_LIMIT_H
#define YAOJUDGE_RESOUCE_LIMIT_H

#include "common.h"

/**
 * @brief Resource limitation context.
 */
struct rsclim_ctxt {
  int real_time;      //!< in milliseconds. 0 for unlimited.
  int cpu_time;       //!< in milliseconds. 0 for unlimited.
  int virtual_memory; //!< in bytes, for RLIMIT_AS. 0 for unlimited.
  int actual_memory;  //!< in bytes, compared with ru_maxrss. 0 for unlimited.
  int stack_memory;   //!< in bptes. 0 for unlimited.
  int output_size;    //!< in bytes. 0 for unlimited.
};
typedef struct rsclim_ctxt *rsclim_ctxt_t;

/**
 * @return 0 on success, 1 otherwise
 */
int apply_resource_limit(perform_ctxt_t ctxt);

/**
 * @return 0 on success, 1 otherwise
 */
int get_usage_after_wait(perform_ctxt_t ctxt);

#endif