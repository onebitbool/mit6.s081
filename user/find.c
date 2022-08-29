#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *name)
{
  char buf[512], *p;

  int fd;
  struct dirent de;
  struct stat st;
  fd = open(path, 0);
  fstat(fd, &st);

  if (st.type == T_FILE) {
    fprintf(2, "parameter should be directory\n");
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de))
  {
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("ls: cannot stat %s\n", buf);
      continue;
    }
    if (st.type == T_DIR && strcmp(de.name, ".") && strcmp(de.name, "..")) {
      find(buf, name);
    }
    if (strcmp(de.name, name))
      continue;
    printf("%s\n", buf);
  }
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "parameter error\n");
  }
  char *path = argv[1];
  char *file_name = argv[2];

  find(path, file_name);
  exit(0);
}