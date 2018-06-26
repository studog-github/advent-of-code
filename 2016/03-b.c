#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int a1, a2, a3;
  int b1, b2, b3;
  int c1, c2, c3;

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
    ret = fscanf(fp, "%d %d %d\n%d %d %d\n%d %d %d", &a1, &a2, &a3, &b1, &b2, &b3, &c1, &c2, &c3);
    if (ret == EOF) {
      break;
    } else if (ret != 9) {
      printf("Error: Expected numbers, scanned %d %d %d %d %d %d %d %d %d %d instead\n", ret, a1, a2, a3, b1, b2, b3, c1, c2, c3);
      return 1;
    }

    if ((a1 + b1 > c1) && (a1 + c1 > b1) && (b1 + c1 > a1))
      possible++;
    if ((a2 + b2 > c2) && (a2 + c2 > b2) && (b2 + c2 > a2))
      possible++;
    if ((a3 + b3 > c3) && (a3 + c3 > b3) && (b3 + c3 > a3))
      possible++;
  }

  printf("%d possible\n", possible);

  return 0;
}
