#include "resouce.h"

int set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  const struct rlimit rl = {.rlim_cur = cur, .rlim_max = max};
  if (setrlimit(type, &rl)) {
    SET_ERRORF("setrlimit failed (type = %d, cur = %d, max = %d)", type,
               (int)cur, (int)max);
    return 1;
  }
  return 0;
}

int apply_resource_limit_rsc(struct rsclim_ctxt *rctxt) {
  if (rctxt->time < 0) {
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

  if (rctxt->time > 0) {
    // upper bound
    int time_ins = rctxt->time / 1000 + !!(rctxt->time % 1000);
    // 实际上只限制了 CPU 的时间，可以看作一个 hard limit，真要干还是得开线程
    if (set_rlimit(RLIMIT_CPU, time_ins, CPU_TIME_H_LIMIT))
      return 1;
    LOG_INFO("set cpu time (RLIMIT_CPU): %ds", time_ins);
  }

  if (rctxt->virtual_memory > 0) {
    // RLIMIT_AS 限制的是虚拟内存的大小，而不是实际用量。比如可以实现 CCF
    // feature：数组开爆
    if (set_rlimit(RLIMIT_AS, rctxt->virtual_memory,
                   max(rctxt->virtual_memory, AS_H_LIMIT)))
      return 1;
    LOG_INFO("set memory limit: %.3lf KB", rctxt->virtual_memory * 1.0 / KB);
  }

  if (rctxt->output_size > 0) {
    if (set_rlimit(RLIMIT_FSIZE, rctxt->output_size,
                   max(rctxt->output_size, FSIZE_H_LIMIT)))
      return 1;
    LOG_INFO("set output limit: %.3lf KB", rctxt->output_size * 1.0 / KB);
  }

  if (rctxt->stack_memory > 0) {
    if (set_rlimit(RLIMIT_STACK, rctxt->stack_memory,
                   max(rctxt->stack_memory, STACK_H_LIMIT)))
      return 1;
    LOG_INFO("set stack memory limit: %.3lf KB",
             rctxt->stack_memory * 1.0 / KB);
  }
  return 0;
}

int get_usage_after_wait(perform_ctxt_t ctxt) {
  struct rusage r2;
  if (getrusage(RUSAGE_CHILDREN, &r2)) {
    SET_ERRORF("test failed");
    return 1;
  }
  // fprint_rusage(log_fp, &r2);
  ctxt->rusage = r2;
  LOG_INFO("get rusage succeed");
  return 0;
}

int apply_resource_limit(perform_ctxt_t ctxt) {
  return apply_resource_limit_rsc(ctxt->rctxt);
}