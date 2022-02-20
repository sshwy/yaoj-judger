#include "yerr.h"

int yerrno = 0;

int yerr(int errnum) {
  yerrno = errnum;
  return errnum;
}

static char yerrmsg[][100] = {
    [E_TKILL_PTHREAD] = "pthread_create failed",
    [E_HELPER_INVALID_LIM] = "unknown limit type",
    [E_ERRFD] = "open err fd failed",
    [E_DUP] = "dup2 failed",
    [E_PIPE] = "pipe failed",
    [E_ARGC] = "invalid number of arguments",
    [E_FORK] = "fork failed",
    [E_SETPGID] = "setpgid failed",
    [E_PGID] = "pgid != interactor's pid!",
    [E_INTACT_EXEC] = "exec not ready",
    [E_CHILD] = "child process not ready",
    [E_WAIT] = "wait failed",
    [E_FILEFD] = "open file fd failed",
    [E_TOKEN] = "invalid token",
    [E_RSC_LIM] = "invalid resource limitation",
    [E_SETRLIMIT] = "setrlimit failed",
    [E_GETRUSAGE] = "get child rusage failed",
    [E_DUPLICATE_FILE] = "duplicated files",
    [E_FCNTL] = "fcntl error",
    [E_FP] = "invalid fp",
    [E_ERROR] = "error",
    [E_POL_IDT] = "invalid indentifier number",
    [E_KAFEL] = "policy compilation failed",
    [E_APP_POL] = "apply policy failed",
    [E_EXEC] = "exec* failed",
};

char *ystrerr(int errnum) { return yerrmsg[errnum]; }