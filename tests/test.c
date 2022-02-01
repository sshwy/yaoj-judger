#include "../src/judger.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **env) {
  log_fp = fopen(".log.local", "w");

  struct policy_ctxt pctxt = create_policy_ctxt("../policy", "policy");
  struct rsclim_ctxt rctxt = {
      .time = 1500,
      .virtual_memory = 30 * MB,
      .actual_memory = 30 * MB,
      .output_size = 1 * KB,
  };
  struct runner_ctxt ectxt = {.argc = argc - 1, .argv = argv + 1, .env = env};
  struct hook_ctxt hctxt = create_hook_ctxt();

  if (log_fp == NULL) {
    ERRNO_EXIT(-1, "log_fp == NULL");
  }

  struct perform_ctxt ctxt;

  perform(&ctxt, pctxt, rctxt, ectxt, hctxt);

  fprint_result(log_fp, &ctxt.result);

  fclose(log_fp);
  return 0;
}