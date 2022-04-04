#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("argc 1\n");
    execl("main.local", "1", "2", (char *)NULL);
  }
  return 0;
}