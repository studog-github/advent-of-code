#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

typedef struct _disc_t {
  int size;
  int start;
} disc_t;

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  disc_t *discs;
  int num_discs;
  int time;

  if (argc < 2)
    filename = "15-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  discs = NULL;
  num_discs = 0;

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

//printf(":%s:\n", line);
    discs = realloc(discs, (num_discs + 1) * sizeof(*discs));
    if (discs == NULL) {
      printf("Error: Out of memory (realloc discs)\n");
      return 1;
    }

#define SKIP_SPACE ({                                             \
          do { i++; } while (line[i] != ' ' && line[i] != '\0');  \
          if (line[i] == '\0') {                                  \
            printf("Error: Unexpected EOL '%s'\n", line);         \
            return 1;                                             \
          }                                                       \
        })

    i = 6; SKIP_SPACE; SKIP_SPACE;
    discs[num_discs].size = atoi(&line[i]);
    SKIP_SPACE; i += 40;
    discs[num_discs].start = atoi(&line[i]);

printf("size: %d start: %d\n", discs[num_discs].size, discs[num_discs].start);
    num_discs++;
  }

  for (time = 0; /**/; time++) {
printf("t[%d]", time);
    for (i = 0; i < num_discs; i++) {
printf(" d[%d] %d", i, (discs[i].start + time + i + 1) % discs[i].size);
      if ((discs[i].start + time + i + 1) % discs[i].size != 0) {
        break;
      }
    }
printf("\n");
    if (i == num_discs)
      break;
  }

  printf("Time: %d\n", time);

  return 0;
}
