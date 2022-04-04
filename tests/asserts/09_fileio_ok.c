#include <stdio.h>

int main() {
  freopen("asserts/aplusb.in", "r", stdin);
  freopen("t.out", "w", stdout);

  int a, b;
  scanf("%d%d", &a, &b);
  printf("%d\n", a + b);
  return 0;
}