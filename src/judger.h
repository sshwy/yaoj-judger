#ifndef YAOJUDGER_H
#define YAOJUDGER_H

#include <linux/filter.h>

#include "../kafel/include/kafel.h"
#include "./common.h"

struct result perform(struct policy_ctxt pctxt, struct rsclim_ctxt rctxt,
                      struct runner_ctxt ectxt);

#endif /* KAFEL_H */