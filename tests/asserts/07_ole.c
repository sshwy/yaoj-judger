#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int a, b;
  scanf("%d%d", &a, &b);
  for (int i = 0; i < 10000000; i++)
    printf("%d\n", a + b);
  return 0;
}