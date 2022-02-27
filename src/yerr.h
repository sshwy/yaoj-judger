#ifndef YAOJ_ERROR_H
#define YAOJ_ERROR_H

#include "common.h"

#define E_TKILL_PTHREAD 1
#define E_HELPER_INVALID_LIM 2
#define E_ERRFD 3
#define E_DUP 4
#define E_PIPE 5
#define E_ARGC 6
#define E_FORK 7
#define E_SETPGID 8
#define E_PGID 9
#define E_INTACT_EXEC 10
#define E_CHILD 11
#define E_WAIT 12
#define E_FILEFD 13
#define E_TOKEN 14
#define E_RSC_LIM 15
#define E_SETRLIMIT 16
#define E_GETRUSAGE 17
#define E_DUPLICATE_FILE 18
#define E_FCNTL 19
#define E_FP 20
#define E_ERROR 21
#define E_POL_IDT 22
#define E_KAFEL 23
#define E_APP_POL 24
#define E_EXEC 25
#define E_BULTIN_POL 26

extern int yerrno;

int yerr(int errnum);
char *ystrerr(int errnum);

#define yerrlog(errnum)                                                        \
  do {                                                                         \
    fprintf(log_fp, RED("ERROR") "(" AT "): %s.\n", ystrerr(errnum));          \
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

#endif