/* parse output of ausyscall */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s [output_file]\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *flistp = popen("ausyscall --dump", "r");
  FILE *foutp = fopen(argv[1], "w");

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

  fclose(foutp);
  fclose(flistp);
  return 0;
}