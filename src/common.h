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

#include "judger.h"
#include "yerr.h"

/// ASCII's red character wrapper
#define RED(s) "\033[31m" s "\033[0m"
/// ASCII's green character wrapper
#define GREEN(s) "\033[32m" s "\033[0m"
/// ASCII's yellow color character wrapper
#define YELLOW(s) "\033[33m" s "\033[0m"
/// ASCII's blue color character wrapper
#define BLUE(s) "\033[34m" s "\033[0m"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
/// location of current line
#define AT __FILE__ ":" TOSTRING(__LINE__)

extern FILE *log_fp;
extern int log_fd, log_level, log_color;
extern char log_buf[1000];

#define LOG(prefix, fmt, args...)                                              \
  do {                                                                         \
    fprintf(log_fp, prefix "(" AT "): " fmt ".\n", ##args);                    \
  } while (0)

#define LOG_CTL(level, colored, origin, fmt, args...)                          \
  do {                                                                         \
    if (level >= log_level) {                                                  \
      if (log_color) {                                                         \
        fprintf(log_fp, colored "(" AT "): " fmt ".\n", ##args);               \
      } else {                                                                 \
        fprintf(log_fp, origin "(" AT "): " fmt ".\n", ##args);                \
      }                                                                        \
    }                                                                          \
  } while (0)
// write(log_fd, log_buf, sizeof(char) * strlen(log_buf));

/// output formatted messages to the file determined by `log_fp`
#define LOG_DEBUG(args...) LOG_CTL(0, BLUE("DEBUG"), "DEBUG", ##args)
#define LOG_INFO(args...) LOG_CTL(1, GREEN("INFO"), "INFO", ##args)
#define LOG_WARN(args...) LOG_CTL(2, YELLOW("WARN"), "WARN", ##args)
#define LOG_ERROR(args...) LOG_CTL(3, RED("ERROR"), "ERROR", ##args)

/**
 * @brief confirm if the condition is true, otherwise exit with code 1.
 *
 * It will directly output error message to stderr.
 */
// #define ASSERT(condition, args...)                                             \
//   do {                                                                         \
//     if (!(condition)) {                                                        \
//       fprintf(stderr, RED("ERROR") "(" AT ") ");                               \
//       fprintf(stderr, ##args);                                                 \
//       exit(1);                                                                 \
//     }                                                                          \
//   } while (0)

#define CPU_TIME_H_LIMIT 300 //!< 5 minutes
#define FSIZE_H_LIMIT (64 * MB)
#define STACK_H_LIMIT (64 * MB)
#define AS_H_LIMIT (512 * MB)
#define RSC_UNLIMITED 0

#define yerrlog(errnum)                                                        \
  do {                                                                         \
    LOG_ERROR("%s.\n", ystrerr(errnum));                                       \
    fflush(log_fp);                                                            \
  } while (0)

#define yreturn(errnum)                                                        \
  do {                                                                         \
    yerrlog(errnum);                                                           \
    return yerr(errnum);                                                       \
  } while (0)

#define yexit(errnum)                                                          \
  do {                                                                         \
    yerrlog(errnum);                                                           \
    exit(errnum);                                                              \
  } while (0)

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
typedef struct perform_ctxt *perform_ctxt_t;

/**
 * @brief Context of perform.
 */
struct perform_ctxt {
  /// pid of the current process
  pid_t pself;
  /// pid of the forked child (It's often assumed that the `perform` will do at
  /// least one fork)
  pid_t pchild;

  int status;                   //!< child process termination status
  struct rusage rusage;         //!< resource usage of child process (getrusage)
  struct perform_result result; //!< perform result of child process

  // these member are not considered to be exposed
  /// pointer at the policy context (used by builtin_hooks).
  struct policy_ctxt *pctxt;
  /// pointer at the resource limitation context (used by builtin_hooks).
  struct rsclim_ctxt *rctxt;
  /// pointer at the runner context.
  struct runner_ctxt *ectxt;
  /// pointer at the hook context.
  struct hook_ctxt *hctxt;
};

int max(int a, int b);

void fprint_result(FILE *fp, struct perform_result *resp);

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