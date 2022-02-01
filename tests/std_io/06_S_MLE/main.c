#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void f(int depth) {
  if (depth > 0)
    f(depth - 1);
}

int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  f(1000000);
  printf("%d\n", a + b);
  return 0;
}