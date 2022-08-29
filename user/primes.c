#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define maxN 35
int INT_LEN = sizeof(int);

/**
 * @brief transfer lpile data to rpile exclusive number that can divide base
**/
void
transfer_pile(int lpile[2], int rpile[2], int base)
{
  int buff;
  while (read(lpile[0], &buff, INT_LEN)) {
    if (buff % base != 0) {
      write(rpile[1], &buff, INT_LEN);
    }
  }
  return;
}

/**
 * @brief print first number in th pile, then transfer the rest of number exclusvie number that can divide base to a new pile. Recursive it for the new pile.
**/
void
primes(int lpile[2])
{
  close(lpile[1]);
  int buff;
  if (read(lpile[0], &buff, INT_LEN) == INT_LEN) {
    fprintf(1, "prime %d\n", buff);
    int p[2];
    pipe(p);
    transfer_pile(lpile, p, buff);
    close(lpile[0]);
    if (fork() == 0) {
      primes(p);
    } else {
      close(p[0]);
      close(p[1]);
      wait(0);
    }
  } else {
    close(lpile[0]);
  }
  exit(0);
}

int
main(int argc, char **argv)
{
  int p[2];
  pipe(p);

  for (int i = 2; i <= maxN; i++) {
    write(p[1], &i, INT_LEN);
  }
  
  if(fork() == 0) {
    primes(p);
  } else {
    close(p[0]);
    close(p[1]);
    wait(0);
  }
  exit(0);
}