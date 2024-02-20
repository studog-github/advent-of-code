#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE  10240
#define LINE_SIZE_S  "10240"

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };

  if (argc < 2)
    filename = "10-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    printf(":%s:\n", line);
  }

  return 0;
}
