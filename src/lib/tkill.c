#include "tkill.h"
#include "common.h"
#include "lib/resouce.h"

void *timeout_killer(void *_tkill_ctxt) { // return void specified!
  struct tkill_ctxt *ctxt = _tkill_ctxt;
  // this is a new thread, kill the process if timeout
  pid_t pid = ctxt->pid;
  int time = ctxt->time;
  // On success, pthread_detach() returns 0; on error, it returns an error
  // number. pthread_self: 获得线程自身的ID
  if (pthread_detach(pthread_self()) != 0) {
    killpg(pid, SIGKILL);
    return NULL;
  }

  // LOG_INFO("timeout_killer: pid=%d timeout=%d\n", pid, time);

  // usleep can't be used, for time args must < 1000ms
  // this may sleep longer that expected, but we will have a check at the end
  if (sleep((unsigned int)((time + 1000) / 1000)) != 0) {
    killpg(pid, SIGKILL);
    return NULL;
  }
  if (killpg(pid, SIGKILL) != 0) {
    return NULL;
  }
  return NULL;
}

int start_timeout_killer(struct tkill_ctxt *tctxtp, pthread_t *tidp) {
  *tidp = 0;
  if (pthread_create(tidp, NULL, timeout_killer, (void *)(tctxtp)) != 0) {
    // make sure pgid has set for child
    killpg(tctxtp->pid, SIGKILL);
    SET_ERRORF("pthread_create failed");
    return 1;
  }
  return 0;
}

int stop_timeout_killer(pthread_t tid) {
  int signal;
  if ((signal = pthread_cancel(tid) != 0)) {
    return signal; // let others handle exception
  };
  return 0;
}

static pthread_t tid = 0;

int start_killer_after_fork(perform_ctxt_t ctxt) {
  struct tkill_ctxt tctxt = {
      .pid = ctxt->pchild,
      .time = ctxt->rctxt->time,
  };
  if (ctxt->rctxt->time != RSC_UNLIMITED) {
    return start_timeout_killer(&tctxt, &tid); // let others handle exception
  }
  return 0;
}

int stop_killer_after_wait(perform_ctxt_t ctxt) {
  if (ctxt->rctxt->time != RSC_UNLIMITED) {
    if (stop_timeout_killer(tid)) {
      LOG_INFO("[ignored] stop timeout killer failed");
      return 0;
    }
  }
  return 0;
}