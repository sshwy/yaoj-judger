#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  for (;;)
    ;
  printf("%d\n", a + b);
  return 0;
}