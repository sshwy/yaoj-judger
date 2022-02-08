#ifndef YAOJUDGE_TIMEOUT_KILL_H
#define YAOJUDGE_TIMEOUT_KILL_H

#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "common.h"

struct tkill_ctxt {
  pid_t pid; //!< monitored process (group) id.
  int time;  //!< int milliseconds.
};

int start_killer_after_fork(perform_ctxt_t ctxt);
int stop_killer_after_wait(perform_ctxt_t ctxt);

#endif