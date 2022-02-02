/**
 * @file common.h
 * @author sshwy (jy.cat@qq.com)
 * @brief Common macros, functions and structure declarations (or definitions).
 * @date 2022-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef YAOJUDGER_COMMON_H
#define YAOJUDGER_COMMON_H

#include <errno.h>
#include <linux/filter.h>
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
      fprintf(stderr, RED(ERROR) "(" __FILE__ "): ");                          \
      fprintf(stderr, ##args);                                                 \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

struct perform_ctxt;
struct policy_ctxt;
struct runner_ctxt;
struct rsclim_ctxt;

/**
 * @brief Kafel policy context
 */
struct policy_ctxt {
  char *dirname; // provide policy search path
  char *policy;  // provide without suffix or path, just name, indicating
                 // [dirname]/[policy].policy
  char *content; // string content of the policy
};
struct policy_ctxt create_policy_ctxt(char *dirname, char *policy);

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
void fprint_result(FILE *fp, struct result *resp);

#define KB (1024)
#define MB (1024 * KB)

#define CPU_TIME_H_LIMIT 300 // 5 minutes
#define FSIZE_H_LIMIT (64 * MB)
#define STACK_H_LIMIT (64 * MB)
#define AS_H_LIMIT (512 * MB)
#define RSC_UNLIMITED 0

void fprint_rusage(FILE *fp, struct rusage *rsp);

char *path_join(const char *first, char seperator, const char *second);

int max(int a, int b);

/**
 * @brief read the whole file and return a string containing its content.
 * @param fp
 * @return char*
 *
 * fp must be opened with at least read access.
 */
char *ftos(FILE *fp);

#endif