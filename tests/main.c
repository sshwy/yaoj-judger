#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// void handle_sigxcpu(int sig) {
//   if (sig == SIGXCPU) {
//     fprintf(stderr, "receive SIXCPU\n");
//   }
// }
void f(int depth) {
  if (depth < 40000)
    f(depth + 1);
}
int main() {
  // if (signal(SIGXCPU, handle_sigxcpu) == SIG_ERR) {
  //   fprintf(stderr, "can't catch SIGXCPU");
  // }

  int a, b;
  scanf("%d%d", &a, &b);
  sleep(1);
  usleep(800 * 1000);
  // for (int i = 0; i < 5e8; i++) // sleep
  //   ;
  // f(0);
  //  int pid = fork();
  //  if (pid == 0) {
  //    printf("child");
  //  } else if (pid > 0) {
  //    printf("parent");
  //  } else {
  //    printf("fork error");
  //  }
  printf("%d\n", a + b);
  return 0;
}