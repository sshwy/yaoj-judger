/* parse output of ausyscall */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

const char command[] = "ausyscall --dump";

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s [output_file]\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *flistp = popen(command, "r");
  FILE *foutp = fopen(argv[1], "w");

  if (flistp == NULL) {
    printf("Error while running ausyscall");
    return EXIT_FAILURE;
  }
  if (foutp == NULL) {
    printf("Error while open output file (errno: %d)", errno);
    return EXIT_FAILURE;
  }

  int id;
  char name[100];
  char line[1000];
  /* can't use fscanf directly! */
  while (fgets(line, sizeof(line), flistp) != NULL) {
    if (sscanf(line, "%d%s", &id, name) == 2 && name[0] != '_') {
      fprintf(foutp, "#define %s %d\n", name, id);
    } else {
      fprintf(stderr, "%s: [ignore] %s", argv[0], line);
    }
  }
  if (pclose(flistp)) { // this will also close the stream
    printf("Error while running \033[33m\"%s\"\033[0m\n", command);
    return EXIT_FAILURE;
  }

  fclose(foutp);
  return 0;
}