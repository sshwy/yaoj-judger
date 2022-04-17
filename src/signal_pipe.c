#include "signal_pipe.h"
#include <unistd.h>

const int length = sizeof(int);

int signal_pipe(int _pipe[2]) { return pipe(_pipe); }

int send_signal(int _pipe[2], int signal) {
  return length == write(_pipe[1], &signal, length);
}

int wait_signal(int _pipe[2]) {
  int signal;
  if (length == read(_pipe[0], &signal, sizeof(int))) {
    return signal;
  } else {
    return -1;
  }
}