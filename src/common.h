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
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>

#include "async_log.h"
#include "judger.h"
#include "yerr.h"

#define CPU_TIME_H_LIMIT 300 //!< 5 minutes
#define FSIZE_H_LIMIT (64 * MB)
#define STACK_H_LIMIT (64 * MB)
#define AS_H_LIMIT (512 * MB)
#define NOFILE_H_LIMIT 50 //!< 最多打开 50 个文件
#define RSC_UNLIMITED 0

/**
 * @brief Context for all runners.
 */
struct runner_ctxt {
  int argc;    //!< number of arguments
  char **argv; //!< argument list end with (char *)NULL
  char **env;  //!< environment var list end with (char *)NULL
};

typedef struct runner_ctxt *runner_ctxt_t;
typedef struct result *result_t;
typedef struct yjudger_ctxt *yjudger_ctxt_t;

/**
 * @brief Context of yjudger.
 */
struct yjudger_ctxt {
  /// pid of the current process
  pid_t pself;
  /// pid of the forked child (It's often assumed that the `perform` will do at
  /// least one fork)
  pid_t pchild;

  int status;                   //!< child process termination status
  struct rusage rusage;         //!< resource usage of child process (getrusage)
  struct yjudger_result result; //!< perform result of child process

  // these member are not considered to be exposed
  /// pointer at the policy context (used by builtin_hooks).
  struct policy_ctxt *pctxt;
  /// pointer at the resource limitation context (used by builtin_hooks).
  struct rsclim_ctxt *rctxt;
  /// pointer at the runner context.
  struct runner_ctxt *ectxt;
  /// pointer at the hook context.
  struct hook_ctxt *hctxt;
  /// tid for tkiller
  pthread_t tid;
};

int max(int a, int b);

void fprint_result(FILE *fp, struct yjudger_result *resp);

// void fprint_rusage(FILE *fp, struct rusage *rsp);

/**
 * @brief Concat two string with `seperator` inserted at the middle of them.
 *
 * Note that if the last character of `first` equals to `seperator`, then it
 * will not be inserted.
 *
 * @return a pointer at a dynamic allocated string
 */
char *path_join(const char *first, char seperator, const char *second);

/**
 * @brief read the whole file and return a string containing its content.
 *
 * `fp` must be valid and opened with at least read access.
 *
 * @return a pointer at a dynamic allocated string
 */
char *ftos(FILE *fp);

/**
 * @brief calculate millisecond value of `timeval`
 */
int to_millisecond(struct timeval tv);

int rmtree_depth1(const char *path);

#endif