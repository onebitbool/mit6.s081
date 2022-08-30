#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  
  int n;
  n = read(0, buf, sizeof(buf));

  char *arg[MAXARG];
  for (int i = 0; i < MAXARG; i++) {
    arg[i] = 0;
  }
  for (int i = 0; i < argc - 1; i++)
  {
    arg[i] = argv[i + 1];
  }

  char *additional_arg[MAXARG];
  int index = 0;
  additional_arg[index++] = buf;
  for (int i = 0; i < n - 1; i++) {
    if (buf[i] == '\n') {
      buf[i] = '\0';
      additional_arg[index++] = buf + i + 1;
    }
  }
  buf[n - 1] = '\0';

  for (int i = 0; i < index; i++) {
    arg[argc - 1] = additional_arg[i];
    if (fork() == 0) {
      exec(argv[1], arg);
    } else {
      wait(0);
    }
  }
  exit(0);
}  