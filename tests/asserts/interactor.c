// interactor.cpp
#include <assert.h>
#include <stdio.h>
#define FOR(i, a, b) for (int i = (a); i <= (b); ++i)

int a, b;
int main(int argc, char **argv) {
  FILE *inf = fopen(argv[1], "r"), *ouf = fopen(argv[2], "w");
  fscanf(inf, "%d%d", &a, &b);
  fclose(inf);
  fprintf(stderr, "a=%d b=%d\n", a, b);
  fflush(stderr);

  assert(a <= b);
  // sleep(5);

  printf("%d\n", b);
  fflush(stdout);

  FOR(_, 1, 30) {
    char x[5];
    int y;
    scanf("%s%d", x, &y);
    fprintf(stderr, "b.cpp: x:[%s], y:[%d]\n", x, y);
    if (x[0] == '?') {
      printf("%d\n", a >= y);
      fflush(stdout);
    } else {
      assert(x[0] == '!');
      if (y == a) {
        fprintf(ouf, "AC\n");
      } else {
        fprintf(ouf, "WA\n");
      }
      return 0;
    }
  }

  fprintf(ouf, "WA\n");
  fclose(ouf);
  return 0;
}