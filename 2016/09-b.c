#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE  10240
#define LINE_SIZE_S  "10240"

unsigned long long decompress(char *line, int len) {
  unsigned long long decompressed_len;
  int i;

  int data_len;
  int repeat_count;

//printf("decompress(%.*s, %d)\n", len, line, len);
  decompressed_len = 0;
  for (i = 0; i < len; i++) {
    if (line[i] < '!' || line[i] > '~') {
      continue;
    } else if (line[i] == '(') {
      i++;
      data_len = atoi(&line[i]);
      do { i++; } while (line[i] != 'x'); i++;
      repeat_count = atoi(&line[i]);
//printf("%dx%d ", data_len, repeat_count);
      do { i++; } while (line[i] != ')'); i++;
      decompressed_len += decompress(&line[i], data_len) * repeat_count;
      i += data_len - 1;
    } else {
      decompressed_len++;
//printf("%c %d\n", line[i], decompressed_len);
    }
  }

//printf("Returning %d\n", decompressed_len);
  return decompressed_len;
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char line[LINE_SIZE + 1] = { 0 };

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

    printf("Decompressed length is %llu\n", decompress(line, strlen(line)));
  }

  return 0;
}
