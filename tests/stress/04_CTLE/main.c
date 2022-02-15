#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int counter = 0;
int main() {
  sleep(1);
  usleep(700000);
  while (1.0 * clock() / CLOCKS_PER_SEC < 1.7) {
    counter = (counter + counter + 1) % 123;
    counter = counter * counter % 123;
    counter = counter / counter % 123;
  }
  printf("%.3lf\n", 1.0 * clock() / CLOCKS_PER_SEC);
  return 0;
}