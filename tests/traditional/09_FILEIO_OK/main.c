#include <stdio.h>

int main() {
  freopen("main.in", "r", stdin);
  freopen("main.out", "w", stdout);

  int a, b;
  scanf("%d%d", &a, &b);
  printf("%d\n", a + b);
  return 0;
}