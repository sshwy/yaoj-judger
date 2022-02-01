#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int a[10000000];

int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  printf("%d\n", a + b);
  return 0;
}