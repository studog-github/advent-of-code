#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int a;
  int b;
  int c;

  int possible;

  if (argc < 2)
    filename = "03-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  possible = 0;

  while (1) {
    ret = fscanf(fp, "%d %d %d\n", &a, &b, &c);
    if (ret == EOF) {
      break;
    } else if (ret != 3) {
      printf("Error: Expected a move, scanned %d %d %d %d  instead\n", ret, a, b, c);
      return 1;
    }

    if ((a + b > c) && (a + c > b) && (b + c > a))
      possible++;
  }

  printf("%d possible\n", possible);

  return 0;
}
