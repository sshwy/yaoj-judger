#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

#include "builtin_hook.h"
#include "common.h"
#include "hook.h"
#include "kafel.h"
#include "lib/policy.h"
#include "lib/resouce.h"
#include "lib/tkill.h"

static struct timeval start, end;

static void timer_before_fork(perform_ctxt_t ctxt) {
  LOG_INFO("get start time.\n");
  gettimeofday(&start, NULL);
}

static void timer_after_wait(perform_ctxt_t ctxt) {
  gettimeofday(&end, NULL);
  ctxt->result.real_time = (int)(end.tv_sec * 1000 + end.tv_usec / 1000 -
                                 start.tv_sec * 1000 - start.tv_usec / 1000);
  LOG_INFO("get end time, real time: %.3lfs\n",
           ctxt->result.real_time / 1000.0);
}

static void init_result_before_fork(perform_ctxt_t ctxt) {
  ctxt->result.code = SE;
  ctxt->result.exit_code = 0;
  ctxt->result.signal = 0;
  ctxt->result.real_memory = 0;
  ctxt->result.real_time = 0;

  LOG_INFO("init ctxt before fork.\n");
}

static void analyze_after_wait(perform_ctxt_t ctxt) {
  ctxt->result.real_memory = ctxt->rusage.ru_maxrss;

  if (WIFSIGNALED(ctxt->status)) {
    LOG_INFO("child process terminated by signal %d.\n",
             WTERMSIG(ctxt->status));

    ctxt->result.signal = WTERMSIG(ctxt->status);

    switch (WTERMSIG(ctxt->status)) {
    case SIGUSR1:
      ctxt->result.code = SE;
      break;
    case SIGSYS:
      ctxt->result.code = DSC;
      break;
    case SIGXCPU:
      ctxt->result.code = TLE;
      break;
    case SIGSEGV:
      if (ctxt->rctxt->virtual_memory == RSC_UNLIMITED)
        ctxt->result.code = RE;
      else
        ctxt->result.code = MLE;
      break;
    case SIGXFSZ:
      ctxt->result.code = OLE;
      break;
    case SIGKILL: // often killed by tkiller
      if (ctxt->rctxt->time != RSC_UNLIMITED &&
          ctxt->result.real_time > ctxt->rctxt->time)
        ctxt->result.code = TLE;
      else
        ctxt->result.code = SE; // someone unknown killed it
      break;
    default:
      ctxt->result.code = OK;
    }

  } else {
    ASSERT(WIFEXITED(ctxt->status), "assertion failed.\n");
    LOG_INFO("child process exited with code %d.\n", WEXITSTATUS(ctxt->status));

    ctxt->result.exit_code = WEXITSTATUS(ctxt->status);
    if (WEXITSTATUS(ctxt->status) == 0) {
      if (ctxt->rctxt->actual_memory != RSC_UNLIMITED &&
          ctxt->result.real_memory * KB > ctxt->rctxt->actual_memory)
        ctxt->result.code = MLE;
      else if (ctxt->rctxt->time != RSC_UNLIMITED &&
               ctxt->result.real_time > ctxt->rctxt->time)
        ctxt->result.code = TLE;
      else
        ctxt->result.code = OK;
    } else {
      ctxt->result.code = ECE;
    }
  }
  LOG_INFO("analyzed result.\n");
}

void register_builtin_hook(struct hook_ctxt *hctxt) {
  // timer_before_fork should be the lastone to invoke
  register_hook(hctxt, BEFORE_FORK, timer_before_fork);
  register_hook(hctxt, BEFORE_FORK, init_result_before_fork);
  register_hook(hctxt, BEFORE_FORK, compile_policy_before_fork);

  register_hook(hctxt, AFTER_FORK, start_killer_after_fork);

  register_hook(hctxt, AFTER_WAIT, analyze_after_wait);
  register_hook(hctxt, AFTER_WAIT, stop_killer_after_wait);
  register_hook(hctxt, AFTER_WAIT, get_usage_after_wait);
  register_hook(hctxt, AFTER_WAIT, timer_after_wait);

  LOG_INFO("register builtin hook. (%d)\n", getpid());
}