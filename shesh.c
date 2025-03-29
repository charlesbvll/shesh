#include <stdio.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char *argv[]) {
  NOB_UNUSED(argc);
  NOB_UNUSED(argv);

  // Disable logs so it doesn't interfere with command outputs
  nob_minimal_log_level = NOB_NO_LOGS;

  // Display prompt
  char hostname[1024];
  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  const char *prompt = nob_temp_sprintf("[%s@%s %s]$ ", getlogin(), hostname,
                                        nob_get_current_dir_temp());
  write(STDIN_FILENO, prompt, strlen(prompt));

  int i = 0;
  char *curr = malloc(256);
  char ch;
  Nob_Cmd cmd = {0};
  while (read(STDIN_FILENO, &ch, 1) > 0) {
    if (i > 255) {
      break;
    }
    if (ch == 'q') {
      break;
    }

    if (ch == '\n') {
      if (strcmp(curr, "exit") == 0) {
        break;
      }
      nob_cmd_append(&cmd, nob_temp_sprintf("%s", curr));
      nob_cmd_run_sync_redirect(cmd, (Nob_Cmd_Redirect){
                                         .fdin = STDIN_FILENO,
                                         .fdout = STDIN_FILENO,
                                         .fderr = STDIN_FILENO,
                                     });
      cmd.count = 0;
      write(STDIN_FILENO, "\n", 1);
      write(STDIN_FILENO, prompt, strlen(prompt));
      memset(curr, 0, 256);
      i = 0;
    } else if (ch == ' ') {
      nob_cmd_append(&cmd, nob_temp_sprintf("%s", curr));
      memset(curr, 0, 256);
      i = 0;
    } else {
      curr[i] = ch;
      ++i;
    }
  }
  cmd.count = 0;
  free(curr);
  return EXIT_SUCCESS;
}
