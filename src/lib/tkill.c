#include "tkill.h"
#include "common.h"
#include "lib/resouce.h"

void *timeout_killer(void *_tkill_ctxt) {
  struct tkill_ctxt *ctxt = _tkill_ctxt;
  // this is a new thread, kill the process if timeout
  pid_t pid = ctxt->pid;
  int time = ctxt->time;
  // On success, pthread_detach() returns 0; on error, it returns an error
  // number. pthread_self: 获得线程自身的ID
  if (pthread_detach(pthread_self()) != 0) {
    kill(pid, SIGKILL);
    return NULL;
  }

  // LOG_INFO("timeout_killer: pid=%d timeout=%d\n", pid, time);

  // usleep can't be used, for time args must < 1000ms
  // this may sleep longer that expected, but we will have a check at the end
  if (sleep((unsigned int)((time + 1000) / 1000)) != 0) {
    kill(pid, SIGKILL);
    return NULL;
  }
  if (kill(pid, SIGKILL) != 0) {
    return NULL;
  }
  return NULL;
}

pthread_t start_timeout_killer(struct tkill_ctxt *tctxtp) {
  pthread_t tid = 0;
  if (pthread_create(&tid, NULL, timeout_killer, (void *)(tctxtp)) != 0) {
    kill(tctxtp->pid, SIGKILL);
    ASSERT(0, "pthread_create failed\n");
  }
  return tid;
}

void stop_timeout_killer(pthread_t tid) {
  int signal;
  if ((signal = pthread_cancel(tid) != 0)) {
    // LOG_INFO("pthread_cancel: return %d\n", signal);
  };
}

static pthread_t tid = 0;
void start_killer_after_fork(perform_ctxt_t ctxt) {
  struct tkill_ctxt tctxt = {.pid = ctxt->pchild, .time = ctxt->rctxt->time};
  if (ctxt->rctxt->time != RSC_UNLIMITED) {
    tid = start_timeout_killer(&tctxt);
  }
}

void stop_killer_after_wait(perform_ctxt_t ctxt) {
  if (ctxt->rctxt->time != RSC_UNLIMITED) {
    stop_timeout_killer(tid);
  }
}