#include "mutex_log.h"
#include "judger.h"
#include "yerr.h"
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;

FILE *log_fp = NULL;
int log_level = 0, log_color = 1;

int flog(const char *fmt, ...) {
  pthread_mutex_lock(&lock);
  int result;
  va_list ptr;
  va_start(ptr, fmt);
  result = vfprintf(log_fp, fmt, ptr);
  va_end(ptr);
  fflush(log_fp);
  pthread_mutex_unlock(&lock);
  return result;
}

int log_init() {
  log_fp = stderr;
  if (pthread_mutex_init(&lock, NULL) != 0) {
    return 1;
  }
  return 0;
}

int log_set(const char *filename, int _log_level, int _with_color) {
  // if (flog_init()) {
  //   yreturn(E_ERROR);
  // }
  log_fp = fopen(filename, "w");
  if (log_fp == NULL) {
    yreturn(E_FP);
  }
  log_level = _log_level;
  log_color = _with_color;
  return 0;
}

void log_close() {
  if (log_fp != stderr && log_fp != stdout) {
    fclose(log_fp);
  }
}
