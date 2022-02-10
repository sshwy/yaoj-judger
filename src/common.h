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

/// wrap the string with ASCII's red color character
#define RED(s) "\033[31m" s "\033[0m"
/// wrap the string with ASCII's green color character
#define GREEN(s) "\033[32m" s "\033[0m"
/// wrap the string with ASCII's yellow color character
#define YELLOW(s) "\033[33m" s "\033[0m"
/// wrap the string with ASCII's blue color character
#define BLUE(s) "\033[34m" s "\033[0m"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

extern FILE *log_fp;
extern int log_fd;
extern char log_buf[1000];

/// output formatted messages to the file determined by `log_fp`
#define LOG_INFO(fmt, args...)                                                 \
  do {                                                                         \
    fprintf(log_fp, GREEN("INFO") "(" AT "): " fmt ".\n", ##args);             \
  } while (0)
// write(log_fd, log_buf, sizeof(char) * strlen(log_buf));

/**
 * @brief confirm if the condition is true, otherwise exit with code 1.
 *
 * It will directly output error message to stderr.
 */
#define ASSERT(condition, args...)                                             \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, RED("ERROR") "(" AT ") ");                               \
      fprintf(stderr, ##args);                                                 \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

extern char errmsg[200];
extern int error_flag;

#define SET_ERRORF(args...)                                                    \
  do {                                                                         \
    sprintf(errmsg, "(" AT "): " args);                                        \
    error_flag = 1;                                                            \
  } while (0)

#define EXIT_WITHMSG()                                                         \
  do {                                                                         \
    fprintf(log_fp, RED("ERROR") "%s.\n", errmsg);                             \
    fflush(log_fp);                                                            \
    exit(1);                                                                   \
  } while (0)

#define CPU_TIME_H_LIMIT 300 //!< 5 minutes
#define FSIZE_H_LIMIT (64 * MB)
#define STACK_H_LIMIT (64 * MB)
#define AS_H_LIMIT (512 * MB)
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
typedef struct perform_ctxt *perform_ctxt_t;

int max(int a, int b);

void fprint_result(FILE *fp, result_t resp);

void fprint_rusage(FILE *fp, struct rusage *rsp);

/**
 * @brief Concat two string with `seperator` inserted at the middle of them.
 *
 * Note that if the last character of `first` equals to `seperator`, then it
 * will not be inserted.
 */
char *path_join(const char *first, char seperator, const char *second);

/**
 * @brief read the whole file and return a string containing its content.
 *
 * fp must be opened with at least read access.
 */
char *ftos(FILE *fp);

/**
 * @brief calculate millisecond value of `timeval`
 */
int to_millisecond(struct timeval tv);

#endif