#ifndef YAOJUDGER_COMMON_H
#define YAOJUDGER_COMMON_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

#define RED(s) "\033[31m" #s "\033[0m"
#define GREEN(s) "\033[32m" #s "\033[0m"
#define YELLOW(s) "\033[33m" #s "\033[0m"

extern FILE *log_fp;

#define LOG_INFO(args...)                                                      \
  do {                                                                         \
    fprintf(log_fp, GREEN(INFO) "(" __FILE__ "): ");                           \
    fprintf(log_fp, ##args);                                                   \
  } while (0)
#define ERRNO_EXIT(signal, args...)                                            \
  do {                                                                         \
    fprintf(log_fp, RED(ERROR) "(" __FILE__ "): ");                            \
    fprintf(log_fp, ##args);                                                   \
    errno = signal;                                                            \
    exit(signal);                                                              \
  } while (0)

#define ASSERT(condition, args...)                                             \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(log_fp, RED(ERROR) "(" __FILE__ "): ");                          \
      fprintf(log_fp, ##args);                                                 \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

struct policy_ctxt {
  char *dirname; // provide policy search path
  char *policy;  // provide without suffix or path, just name, indicating
                 // [dirname]/[policy].policy
};

// 理论上所有 runner 的接口应该是一样的（可变参数）
struct runner_ctxt {
  int argc;
  char **argv, **env;
};

struct sock_fprog compile_policy(struct policy_ctxt pctxt);

enum result_code {
  OK = 0, // all correct
  RE,     // runtime error
  MLE,    // memory limit exceed
  TLE,    // time limit exceed
  OLE,    // output limit exceed
  SE,     // system error
  DSC,    // dangerous system call
  ECE,    // exit code exception
};

struct result {
  enum result_code code;
  int signal;      // terminate signal raised by child process
  int exit_code;   // exit code of child process
  int real_time;   // in milliseconds.
  int real_memory; // in kb.
};

#define KB (1024)
#define MB (1024 * KB)

#define CPU_TIME_H_LIMIT 300 // 5 minutes
#define FSIZE_H_LIMIT (64 * MB)
#define AS_H_LIMIT (512 * MB)

#define RSC_UNLIMITED 0
struct rsclim_ctxt {
  int time;           // in milliseconds. 0 for unlimited
  int virtual_memory; // in bytes, for RLIMIT_AS. 0 for unlimited
  int actual_memory;  // in bytes, compared with ru_maxrss. 0 for unlimited
  int output_size;    // in bytes. 0 for unlimited
};

void fprint_result(FILE *fp, struct result *resp);
void fprint_rusage(FILE *fp, struct rusage *rsp);

char *path_join(const char *first, const char *second);

#endif