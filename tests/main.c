#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// void handle_sigxcpu(int sig) {
//   if (sig == SIGXCPU) {
//     fprintf(stderr, "receive SIXCPU\n");
//   }
// }
int nd[1000000];
int main() {
  // if (signal(SIGXCPU, handle_sigxcpu) == SIG_ERR) {
  //   fprintf(stderr, "can't catch SIGXCPU");
  // }

  int a, b;
  scanf("%d%d", &a, &b);
  for (int i = 0; i < 100000; i++)
    nd[i] = i + 1;
  // for (;;) // sleep
  //   ;
  // int pid = fork();
  // if (pid == 0) {
  //   printf("child");
  // } else if (pid > 0) {
  //   printf("parent");
  // } else {
  //   printf("fork error");
  // }
  printf("%d\n", a + b);
  return 1;
}