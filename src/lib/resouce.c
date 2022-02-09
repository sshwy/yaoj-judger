#include "resouce.h"
#include "judger.h"

int set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  if (max < cur)
    max = cur;
  const struct rlimit rl = {
      .rlim_cur = cur,
      .rlim_max = max,
  };
  if (setrlimit(type, &rl)) {
    SET_ERRORF("setrlimit failed (type = %d, cur = %d, max = %d)", type,
               (int)cur, (int)max);
    return 1;
  }
  return 0;
}

int upperbound(double x) {
  int ix = x;
  if (x > ix)
    return ix + 1;
  else
    return ix;
}

int apply_resource_limit_rsc(struct rsclim_ctxt *rctxt) {
  if (rctxt->cpu_time < 0) {
    SET_ERRORF("invalid time");
    return 1;
  }
  if (rctxt->virtual_memory < 0) {
    SET_ERRORF("invalid virtual_memory");
    return 1;
  }
  if (rctxt->actual_memory < 0) {
    SET_ERRORF("invalid actual_memory");
    return 1;
  }
  if (rctxt->stack_memory < 0) {
    SET_ERRORF("invalid stack_memory");
    return 1;
  }
  if (rctxt->output_size < 0) {
    SET_ERRORF("invalid output_size");
    return 1;
  }

  if (rctxt->cpu_time > 0) {
    int time_ins = upperbound(rctxt->cpu_time / 1000.0);
    // limit cpu running time
    if (set_rlimit(RLIMIT_CPU, time_ins, CPU_TIME_H_LIMIT))
      return 1;
    LOG_INFO("set cpu time (RLIMIT_CPU): %ds", time_ins);
  }

  if (rctxt->virtual_memory > 0) {
    // RLIMIT_AS 限制的是虚拟内存的大小，而不是实际用量。比如可以实现 CCF
    // feature：数组开爆
    if (set_rlimit(RLIMIT_AS, rctxt->virtual_memory, AS_H_LIMIT))
      return 1;
    LOG_INFO("set memory limit: %.3lfMB", rctxt->virtual_memory * 1.0 / MB);
  }

  if (rctxt->output_size > 0) {
    if (set_rlimit(RLIMIT_FSIZE, rctxt->output_size, FSIZE_H_LIMIT))
      return 1;
    LOG_INFO("set output limit: %.3lfMB", rctxt->output_size * 1.0 / MB);
  }

  if (rctxt->stack_memory > 0) {
    if (set_rlimit(RLIMIT_STACK, rctxt->stack_memory, STACK_H_LIMIT))
      return 1;
    LOG_INFO("set stack memory limit: %.3lfMB", rctxt->stack_memory * 1.0 / MB);
  }
  return 0;
}

int get_usage_after_wait(perform_ctxt_t ctxt) {
  struct rusage r2;
  if (getrusage(RUSAGE_CHILDREN, &r2)) {
    SET_ERRORF("get child's rusage failed");
    return 1;
  }
  // fprint_rusage(log_fp, &r2);
  ctxt->rusage = r2;
  ctxt->result.cpu_time = to_millisecond(ctxt->rusage.ru_utime) +
                          to_millisecond(ctxt->rusage.ru_stime);
  LOG_INFO("get rusage succeed");
  return 0;
}

int apply_resource_limit(perform_ctxt_t ctxt) {
  return apply_resource_limit_rsc(ctxt->rctxt);
}