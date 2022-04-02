#include "sys/resource.h"
#include <stdio.h>

int set_rlimit(__rlimit_resource_t type, rlim_t cur, rlim_t max) {
  if (max < cur)
    max = cur;
  const struct rlimit rl = {
      .rlim_cur = cur,
      .rlim_max = max,
  };
  return setrlimit(type, &rl);
}

int main() {
  printf("gg\n");
  set_rlimit(RLIMIT_CPU, 60, 60);
  int t = set_rlimit(RLIMIT_CPU, 50, 70);
  printf("t = %d\n", t);
  return t;
}