#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "judger.h"

FILE *log_fp = NULL;
int log_fd = -1;
char log_buf[1000];

void log_set(const char *filename) {
  log_fp = fopen(filename, "w");
  ASSERT(log_fp != NULL, "invalid log file\n");
  log_fd = fileno(log_fp);
}

char errmsg[200];
int error_flag = 0;

int max(int a, int b) { return a > b ? a : b; }

char *path_join(const char *first, char seperator, const char *second) {
  size_t flen = strlen(first);
  size_t slen = strlen(second);
  ASSERT(flen <= SIZE_MAX - 2, "assertion failed\n");        // overflow
  ASSERT(flen + 2 <= SIZE_MAX - slen, "assertion failed\n"); // overflow
  if (flen > 0 && first[flen - 1] == seperator)
    --flen; // remove trailing '/'
  size_t len = flen + slen + 1;
  char *rv = malloc(len + 1);
  ASSERT(rv != NULL, "assertion failed\n"); // OOM
  memcpy(rv, first, flen);
  rv[flen] = seperator;
  memcpy(rv + flen + 1, second, slen);
  rv[len] = '\0';
  return rv;
}

void fprint_result(FILE *fp, struct result *pres) {
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
          pres->cpu_time / 1000.0, pres->real_memory / 1000.0, pres->code,
          pres->signal, pres->exit_code);
}

void log_print_result(struct result *pres) { fprint_result(log_fp, pres); }
void log_close() { fclose(log_fp); }

void fprint_rusage(FILE *fp, struct rusage *rsp) {
  fprintf(fp, "ru_utime: %ld %ld\n", rsp->ru_utime.tv_sec,
          rsp->ru_utime.tv_usec);
  fprintf(fp, "ru_stime: %ld %ld\n", rsp->ru_stime.tv_sec,
          rsp->ru_stime.tv_usec);
#define PRINT(prop, unit) fprintf(fp, #prop ": %ld " unit "\n", rsp->prop)
  PRINT(ru_maxrss, "kb"); // ru_maxrss 统计的是实际使用的内存量
  // PRINT(ru_ixrss, "kb-s");
  // PRINT(ru_idrss, "kb-s");
}

char *ftos(FILE *fp) {
  ASSERT(fp != NULL, "ftos: invalid fp\n");
  fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp);
  fseek(fp, 0, SEEK_SET); // rewind(fp);
  char *s = malloc(size + 1);
  fread(s, size, 1, fp);
  s[size] = 0;
  fclose(fp);
  return s;
}

const char result_code_name[][10] = {
    [OK] = "OK",   [RE] = "RE", [MLE] = "MLE", [TLE] = "TLE",
    [OLE] = "OLE", [SE] = "SE", [DSC] = "DSC", [ECE] = "ECE",
};

enum result_code atorc(char *arg) {
  for (int i = 0; i < 8; i++) {
    if (strcmp(arg, result_code_name[i]) == 0)
      return i;
  }
  ASSERT(0, "\"%s\" doesn't match any result code name.\n", arg);
  return 0;
}

int to_millisecond(struct timeval tv) {
  return (int)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}