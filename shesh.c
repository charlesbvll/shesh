#include <stdio.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char *argv[]) {
  NOB_UNUSED(argc);
  NOB_UNUSED(argv);
  const char *prompt = "[carlitos@localhost]$ ";
  write(STDIN_FILENO, prompt, strlen(prompt));

  int i = 0;
  char *curr = malloc(256);
  char ch;
  while (read(STDIN_FILENO, &ch, 1) > 0) {
    if (i > 255) {
      return EXIT_SUCCESS;
    }
    curr[i] = ch;
    ++i;
  }
  return EXIT_SUCCESS;
}
