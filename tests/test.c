#include "../src/judger.h"
#include <stdlib.h>

int main(int argc, char **argv, char **env) {
  struct policy_ctxt pctxt = {.dirname = "../policy", .policy = "policy"};
  struct rsclim_ctxt rctxt = {
      .time = 1500,
      .virtual_memory = 30 * MB,
      .actual_memory = 30 * MB,
      .output_size = 1 * KB,
  };

  log_fp = fopen(".log.local", "w");
  if (log_fp == NULL) {
    ERRNO_EXIT(-1, "log_fp == NULL");
  }

  struct runner_ctxt ectxt = {.argc = argc - 1, .argv = argv + 1, .env = env};

  struct result res = perform(pctxt, rctxt, ectxt);

  fprint_result(log_fp, &res);

  return 0;
}