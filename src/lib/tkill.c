#define _GNU_SOURCE

#include <pthread.h>
#include <signal.h>
#include <sys/resource.h>

#include "mutex_log.h"
#include "common.h"
#include "judger.h"
#include "lib/resource.h"
#include "tkill.h"

struct tkill_ctxt {
  pid_t pid; //!< monitored process (group) id.
  int time;  //!< int milliseconds.
};

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

  // usleep can't be used, for time args must < 1000ms
  // this may sleep longer that expected, but we will have a check at the end
  if (sleep((unsigned int)((time + 1000) / 1000)) != 0) {
    killpg(pid, SIGKILL);
    return NULL;
  }
  if (killpg(pid, SIGKILL) != 0) {
    return NULL;
  }
  pthread_exit(NULL);
  return NULL;
}

int stop_timeout_killer(pthread_t tid) {
  LOG_DEBUG("stop tkiller");
  int signal = pthread_cancel(tid);
  // https://stackoverflow.com/a/8706759/18065225
  signal |= pthread_join(tid, NULL);
  if (signal != 0) {
    return signal; // let others handle exception
  };
  return 0;
}

#define TKILL_STACK_SIZE 32768 // 32kb

int start_killer_after_fork(yjudger_ctxt_t ctxt) {
  if (ctxt->rctxt->real_time == RSC_UNLIMITED)
    return 0;

  static pthread_t tid = 0;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, TKILL_STACK_SIZE);

  struct tkill_ctxt tctxt = {
      .pid = ctxt->pchild,
      .time = ctxt->rctxt->real_time,
  };

  int flag = pthread_create(&tid, &attr, timeout_killer, (void *)(&tctxt));
  // if (flag == EAGAIN) { // retry once
  //   LOG_WARN(
  //       "tkill create failed EAGAIN, retry with double stack size (%d
  //       bytes)\n", TKILL_STACK_SIZE * 2);
  //   pthread_attr_init(&attr);
  //   pthread_attr_setstacksize(&attr, TKILL_STACK_SIZE * 2);
  //   flag = pthread_create(&tid, NULL, timeout_killer, (void *)(&tctxt));
  // }
  // if (flag != 0) {
  //   // make sure pgid has set for child
  //   killpg(tctxt.pid, SIGKILL);
  //   LOG_ERROR("tkill create failed(%d)", flag);
  //   struct rlimit rl;
  //   getrlimit(RLIMIT_NPROC, &rl);
  //   LOG_DEBUG("nproc limit: soft=%d, hard=%d", rl.rlim_cur, rl.rlim_max);
  //   yreturn(E_TKILL_PTHREAD);
  // }
  LOG_DEBUG("tkill create succeed (flag = %d)", flag);
  ctxt->tid = tid; // maybe not thread safe
  return 0;
}

int stop_killer_after_wait(yjudger_ctxt_t ctxt) {
  if (ctxt->rctxt->real_time == RSC_UNLIMITED)
    return 0;
  if (ctxt->tid != 0) {
    if (stop_timeout_killer(ctxt->tid)) {
      LOG_WARN("[ignored] stop timeout killer failed");
      return 0;
    }
  }
  return 0;
}