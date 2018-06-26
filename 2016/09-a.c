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
  int i;
  int decompressed_len;
  int data_len;
  int repeat_count;

  if (argc < 2)
    filename = "09-a-input";
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

    decompressed_len = 0;
    i = 0;
    while (line[i]) {
      if (line[i] < '!' || line[i] > '~') {
        i++;
        continue;
      } else if (line[i] == '(') {
        i++;
        data_len = atoi(&line[i]);
        do { i++; } while (line[i] != 'x'); i++;
        repeat_count = atoi(&line[i]);
        do { i++; } while (line[i] != ')'); i++;
        i += data_len;
        decompressed_len += data_len * repeat_count;
      } else {
        i++;
        decompressed_len++;
      }
    }

    printf("Decompressed length is %d\n", decompressed_len);
  }

  return 0;
}
