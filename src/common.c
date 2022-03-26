#define _GNU_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// POSIX dependencies
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "judger.h"

FILE *log_fp = NULL;
int log_fd = -1;
char log_buf[1000];

int log_set(const char *filename) {
  log_fp = fopen(filename, "w");
  if (log_fp == NULL) {
    yreturn(E_FP);
  }
  log_fd = fileno(log_fp);
  return 0;
}

char *path_join(const char *first, char seperator, const char *second) {
  size_t flen = strlen(first);
  size_t slen = strlen(second);
  if (flen > 0 && first[flen - 1] == seperator)
    --flen; // remove trailing '/'
  size_t len = flen + slen + 1;
  char *rv = malloc(len + 1);
  // ASSERT(rv != NULL, "assertion failed\n"); // OOM
  memcpy(rv, first, flen);
  rv[flen] = seperator;
  memcpy(rv + flen + 1, second, slen);
  rv[len] = '\0';
  return rv;
}

void fprint_result(FILE *fp, struct perform_result *pres) {
  char code_name[10][30] = {
      [OK] = "OK",
      [RE] = "Runtime Error",
      [TLE] = "Time Limit Exceed",
      [MLE] = "Memory Limit Exceed",
      [DSC] = "Dangerous System Call",
      [SE] = "System Error",
      [OLE] = "Output Limit Exceed",
      [ECE] = "Exit Code Exception",
  };
  fprintf(fp,
          "result: \033[33m\"%s\"\033[0m "
          "real_time=%.3lfs "
          "cpu_time=%.3lfs "
          "real_memory=%.3lfMB "
          "code=%d signal=%d "
          "exit_code=%d\n",
          code_name[pres->code], pres->real_time / 1000.0,
          pres->cpu_time / 1000.0, pres->real_memory / 1000000.0, pres->code,
          pres->signal, pres->exit_code);
}

void log_print_result(struct perform_result *pres) {
  fprint_result(log_fp, pres);
}
void log_close() { fclose(log_fp); }

// void fprint_rusage(FILE *fp, struct rusage *rsp) {
//   fprintf(fp, "ru_utime: %ld %ld\n", rsp->ru_utime.tv_sec,
//           rsp->ru_utime.tv_usec);
//   fprintf(fp, "ru_stime: %ld %ld\n", rsp->ru_stime.tv_sec,
//           rsp->ru_stime.tv_usec);
// #define PRINT(prop, unit) fprintf(fp, #prop ": %ld " unit "\n", rsp->prop)
//   PRINT(ru_maxrss, "kb"); // ru_maxrss 统计的是实际使用的内存量
//   PRINT(ru_ixrss, "kb-s");
//   PRINT(ru_idrss, "kb-s");
// }

char *ftos(FILE *fp) {
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET); // rewind(fp);
  char *s = malloc(size + 1);
  fread(s, size, 1, fp);
  s[size] = 0;
  fclose(fp);
  return s;
}

int to_millisecond(struct timeval tv) {
  return (int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int rmtree_depth1(const char *path) {
  char *full_path;
  DIR *dir;
  struct stat stat_path, stat_entry;
  struct dirent *entry;

  // stat for the path
  stat(path, &stat_path);

  // if path does not exists or is not dir - exit with status -1
  if (S_ISDIR(stat_path.st_mode) == 0) {
    yreturn(E_ERROR);
  }

  // if not possible to read the directory for this user
  if ((dir = opendir(path)) == NULL) {
    yreturn(E_ERROR);
  }

  // iteration through entries in the directory
  while ((entry = readdir(dir)) != NULL) {

    // skip entries "." and ".."
    if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
      continue;

    // determinate a full path of an entry
    full_path = path_join(path, '/', entry->d_name);

    // stat for the entry
    stat(full_path, &stat_entry);

    // recursively remove a nested directory
    // if (S_ISDIR(stat_entry.st_mode) != 0) {
    //   if (rmtree(full_path))
    //     yreturn(yerrno);
    //   continue;
    // }

    // remove a file object
    if (unlink(full_path) == 0) {
      LOG_DEBUG("removed a file: %s", full_path);
    } else {
      yreturn(E_ERROR);
    }
    free(full_path);
  }

  // remove the devastated directory and close the object of it
  if (rmdir(path) == 0) {
    LOG_DEBUG("removed a directory: %s", path);
  } else {
    yreturn(E_ERROR);
  }

  closedir(dir);
  return 0;
}
