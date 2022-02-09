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

void set_logfile(const char *filename);

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

enum result_code {
  OK = 0, //!< all correct
  RE,     //!< runtime error
  MLE,    //!< memory limit exceed
  TLE,    //!< time limit exceed
  OLE,    //!< output limit exceed
  SE,     //!< system error
  DSC,    //!< dangerous system call
  ECE,    //!< exit code exception
};

/**
 * @brief transform string code name (uppercase) to corresponding code.
 * @param arg
 * @return int
 */
enum result_code atorc(char *arg);

/**
 * @brief Context for all runners.
 */
struct runner_ctxt {
  int argc;    //!< number of arguments
  char **argv; //!< argument list end with (char *)NULL
  char **env;  //!< environment var list end with (char *)NULL
};
typedef struct runner_ctxt *runner_ctxt_t;

/**
 * @brief Describe result of a terminated process.
 */
struct result {
  /// return code of the judgement (often set during perform)
  enum result_code code;
  int signal;      //!< terminate signal raised by child process
  int exit_code;   //!< exit code of child process
  int real_time;   //!< in milliseconds.
  int cpu_time;    //!< in milliseconds.
  int real_memory; //!< in kb.
};
void fprint_result(FILE *fp, struct result *resp);

#define KB (1024)
#define MB (1024 * KB)

#define CPU_TIME_H_LIMIT 300 //!< 5 minutes
#define FSIZE_H_LIMIT (64 * MB)
#define STACK_H_LIMIT (64 * MB)
#define AS_H_LIMIT (512 * MB)
#define RSC_UNLIMITED 0

void fprint_rusage(FILE *fp, struct rusage *rsp);

char *path_join(const char *first, char seperator, const char *second);

int max(int a, int b);

/**
 * @brief read the whole file and return a string containing its content.
 *
 * fp must be opened with at least read access.
 */
char *ftos(FILE *fp);

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

int to_millisecond(struct timeval tv);

typedef struct perform_ctxt *perform_ctxt_t;

#endif