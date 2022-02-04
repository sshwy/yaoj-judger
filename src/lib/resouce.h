#ifndef YAOJUDGE_RESOUCE_LIMIT_H
#define YAOJUDGE_RESOUCE_LIMIT_H

#include "common.h"

void apply_resource_limit(const struct rsclim_ctxt *rctxt);

void get_usage_after_wait(perform_ctxt_t ctxt);

#endif