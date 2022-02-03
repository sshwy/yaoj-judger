#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  sleep(1);
  printf("%d %d\n", a + b, getpid());
  return 0;
}