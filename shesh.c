#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

void append_space_sep_cmd(Nob_Cmd *cmd, const char *line, char *tmp) {
  size_t total = 0;
  size_t curr = 0;
  bool space = false;
  while (total < strlen(line)) {
    if (line[total] != ' ') {
      tmp[curr] = line[total];
      total += 1;
      curr += 1;
    } else {
      total += 1;
      space = true;
      tmp[curr] = '\0';
      nob_cmd_append(cmd, nob_temp_sprintf("%s", tmp));
      curr = 0;
      memset(tmp, 0, 256);
    }
  }
  tmp[curr] = '\0';
  if (!space) {
    strcpy(tmp, line);
  }
  nob_cmd_append(cmd, nob_temp_sprintf("%s", tmp));
}

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
  Nob_Cmd cmd = {0};
  char *tmp = malloc(256);
  while (!feof(stdin)) {
    const char *line = readline(prompt);
    if (strcmp(line, "exit") == 0 || strcmp(line, "q") == 0) {
      break;
    }
    append_space_sep_cmd(&cmd, line, tmp);
    nob_cmd_run_sync_redirect_and_reset(&cmd, (Nob_Cmd_Redirect){
                                                  .fdin = STDIN_FILENO,
                                                  .fdout = STDIN_FILENO,
                                                  .fderr = STDIN_FILENO,
                                              });
  }
  free(tmp);
  return EXIT_SUCCESS;
}
