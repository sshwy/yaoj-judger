// a.cpp
#include <assert.h>
#include <stdio.h>

int check(int x) {
  printf("? %d\n", x); // check if ? >= x
  fflush(stdout);
  fprintf(stderr, "? %d\n", x); // check if ? >= x
  assert(scanf("%d", &x) != EOF);
  return x == 1;
}
int main() {
  return 1; // exit in advance
  int n;
  assert(scanf("%d", &n) != EOF);
  fprintf(stderr, "a.cpp: n %d\n", n);
  int l = 1, r = n, mid;
  while (l < r) {
    mid = (l + r + 1) >> 1;
    if (check(mid))
      l = mid;
    else
      r = mid - 1;
  }
  printf("! %d\n", l);
  fprintf(stderr, "! %d\n", l);
  return 0;
}
