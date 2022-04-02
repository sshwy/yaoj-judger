#ifndef YAOJ_JUDGER_ASYNC_LOG_H
#define YAOJ_JUDGER_ASYNC_LOG_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

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
extern int log_level, log_color;

#define LOG_CTL(level, colored, origin, fmt, args...)                          \
  do {                                                                         \
    if (level >= log_level) {                                                  \
      if (log_color) {                                                         \
        flog(colored "(" AT "): " fmt ".\n", ##args);                          \
      } else {                                                                 \
        flog(origin "(" AT "): " fmt ".\n", ##args);                           \
      }                                                                        \
    }                                                                          \
  } while (0)

#define LOG_DEBUG(args...) LOG_CTL(0, BLUE("DEBUG"), "DEBUG", ##args)
#define LOG_INFO(args...) LOG_CTL(1, GREEN("INFO"), "INFO", ##args)
#define LOG_WARN(args...) LOG_CTL(2, YELLOW("WARN"), "WARN", ##args)
#define LOG_ERROR(args...) LOG_CTL(3, RED("ERROR"), "ERROR", ##args)

int flog(const char *fmt, ...);

#endif