#include "resouce.h"

void set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  const struct rlimit rl = {.rlim_cur = cur, .rlim_max = max};
  ASSERT(setrlimit(type, &rl) == 0, "setrlimit failed.\n");
}

void apply_resource_limit_rsc(struct rsclim_ctxt *rctxt) {
  ASSERT(rctxt->time >= 0, "invalid cpu_time\n");
  ASSERT(rctxt->virtual_memory >= 0, "invalid virtual_memory\n");
  ASSERT(rctxt->actual_memory >= 0, "invalid actual_memory\n");
  ASSERT(rctxt->output_size >= 0, "invalid output_size\n");
  ASSERT(rctxt->stack_memory >= 0, "invalid stack_memory\n");

  if (rctxt->time > 0) {
    // upper bound
    int time_ins = rctxt->time / 1000 + !!(rctxt->time % 1000);
    // 实际上只限制了 CPU 的时间，可以看作一个 hard limit，真要干还是得开线程
    set_rlimit(RLIMIT_CPU, time_ins, CPU_TIME_H_LIMIT);
    LOG_INFO("set cpu time (RLIMIT_CPU): %ds\n", time_ins);
  }

  if (rctxt->virtual_memory > 0) {
    // RLIMIT_AS 限制的是虚拟内存的大小，而不是实际用量。比如可以实现 CCF
    // feature：数组开爆
    set_rlimit(RLIMIT_AS, rctxt->virtual_memory,
               max(rctxt->virtual_memory, AS_H_LIMIT));
    LOG_INFO("set memory limit: %.3lf KB\n", rctxt->virtual_memory * 1.0 / KB);
  }

  if (rctxt->output_size > 0) {
    set_rlimit(RLIMIT_FSIZE, rctxt->output_size,
               max(rctxt->output_size, FSIZE_H_LIMIT));
    LOG_INFO("set output limit: %.3lf KB\n", rctxt->output_size * 1.0 / KB);
  }

  if (rctxt->stack_memory > 0) {
    set_rlimit(RLIMIT_STACK, rctxt->stack_memory,
               max(rctxt->stack_memory, STACK_H_LIMIT));
    LOG_INFO("set stack memory limit: %.3lf KB\n",
             rctxt->stack_memory * 1.0 / KB);
  }
}

void get_usage_after_wait(perform_ctxt_t ctxt) {
  struct rusage r2;
  ASSERT(getrusage(RUSAGE_CHILDREN, &r2) == 0, "test failed.");
  fprint_rusage(log_fp, &r2);
  ctxt->rusage = r2;
}

void apply_resource_limit(perform_ctxt_t ctxt) {
  apply_resource_limit_rsc(ctxt->rctxt);
}