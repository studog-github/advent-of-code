#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

#define SKIP_TO(c) ({                                             \
          do { i++; } while (line[i] != c && line[i] != '\0');    \
          if (line[i] == '\0') {                                  \
            printf("Error: Unexpected EOL '%s'\n", line);         \
            return 1;                                             \
          }                                                       \
        })
#define SKIP_OVER(c) ({     \
          SKIP_TO(c); i++;  \
        })

static inline void **malloc2d(int rows, int cols, size_t size)
{
  void **arr;
  int i;
  size_t header_size;
  size_t data_size;

  header_size = rows * sizeof(void *);
  data_size = rows * cols * size;

  arr = malloc(header_size + data_size);
  if (arr == NULL) {
    return arr;
  }

  for (i = 0; i < rows; i++) {
    arr[i] = ((unsigned char *)arr) + header_size + (i * cols * size);
  }

  return arr;
}

#define DEFAULT_INPUT "20-a-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };

  if (argc < 2)
    filename = DEFAULT_INPUT;
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
