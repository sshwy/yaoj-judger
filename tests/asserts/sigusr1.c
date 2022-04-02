#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

int counter = 0;
int main() {
  raise(SIGUSR1);
  return 0;
}