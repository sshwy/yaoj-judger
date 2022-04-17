#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd[10];
  for (int i = 0; i < 10; i++) {
    fd[i] = open("/dev/null", O_WRONLY);
    printf("fd = %d\n", fd[i]);
    if (fd[i] < 0)
      return 1;
  }
  for (int i = 0; i < 10; i++) {
    close(fd[i]);
  }
  return 0;
}