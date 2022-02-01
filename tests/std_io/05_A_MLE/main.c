#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int c[10000000];
int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  for (int i = 0; i < 1000000; i++)
    c[i] = i;
  printf("%d\n", a + b);
  return 0;
}