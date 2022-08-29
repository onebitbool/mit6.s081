#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  char buf = 'b';
  int p2c[2];
  int c2p[2];
  pipe(p2c);
  pipe(c2p);

  if (fork() == 0){ //child process
    close(p2c[1]);
    close(c2p[0]);

    if (read(p2c[0], &buf, sizeof(char)) == sizeof(char)) {
        fprintf(1, "%d: received ping\n", getpid());
    }

    write(c2p[1], &buf, sizeof(char));

    close(p2c[0]);
    close(c2p[1]);
    exit(0);
  } else {
    close(p2c[0]);
    close(c2p[1]);

    write(p2c[1], &buf, sizeof(char));
    
    if (read(c2p[0], &buf, sizeof(char)) == sizeof(char)) {
        fprintf(1, "%d: received pong\n", getpid());
    }

    close(p2c[1]);
    close(c2p[0]);
    exit(0);
  }
}