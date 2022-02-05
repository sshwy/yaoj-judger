/**
 * @file traditional.c
 * @author sshwy (jy.cat@qq.com)
 * @brief Child process for OI's traditional problem
 * @date 2022-02-04
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "child_proc.h"
#include "lib/policy.h"
#include "lib/resouce.h"
#include "runner.h"

void child_prework(perform_ctxt_t ctxt) {
  LOG_INFO("perform child (%d)\n", ctxt->pchild);
  // sleep(1); // simulate heavy work
  runner_prework(ctxt->ectxt);
  apply_resource_limit(ctxt);
  apply_policy(ctxt);
  fflush(log_fp);
  fclose(log_fp); // !!!important
}

void child_run(perform_ctxt_t ctxt) { runner_run(ctxt->ectxt); }