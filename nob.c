#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);
  Nob_Cmd cmd = {0};
  nob_cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-g", "-o", "shesh", "shesh.c",
                 "-I/opt/homebrew/Cellar/readline/8.2.13/include",
                 "-D_DARWIN_C_SOURCE",
                 "-L/opt/homebrew/Cellar/readline/8.2.13/lib", "-lreadline");
  if (!nob_cmd_run_sync(cmd))
    return 1;
  return 0;
}
