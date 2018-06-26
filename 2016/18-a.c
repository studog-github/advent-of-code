#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"


static void display_floor(char *floor, int rows, int cols)
{
  int i;
  int j;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      printf("%c", *(floor + (i * cols) + j));
    }
    printf("\n");
  }
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  int num_rows;
  int num_cols;
  int row;
  int col;
  char left;
  char center;
  char right;

  char *floor;
  int num_safe_spaces;

  if (argc < 2)
    filename = "18-a-input";
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

//printf(":%s:\n", line);
  }

#define SKIP_SPACE ({                                             \
          do { i++; } while (line[i] != ' ' && line[i] != '\0');  \
          if (line[i] == '\0') {                                  \
            printf("Error: Unexpected EOL '%s'\n", line);         \
            return 1;                                             \
          }                                                       \
        })

  num_safe_spaces = 0;

  i = 0;
  num_rows = atoi(line);
  SKIP_SPACE; i++;
  num_cols = strlen(&line[i]);

  floor = malloc(num_rows * num_cols * sizeof(*floor));
  if (floor == NULL) {
    printf("Error: Out of memory (floor)\n");
    return 1;
  }
  memset(floor, '.', num_rows * num_cols * sizeof(*floor));

  for (col = 0; col < num_cols && line[i]; i++, col++) {
    *(floor + col) = line[i];
    if (line[i] == '.')
      num_safe_spaces++;
  }

  for (row = 1; row < num_rows; row++) {
    for (col = 0; col < num_cols; col++) {
      // row - 1 always valid
      if (col == 0)
        left = '.';
      else
        left = *(floor + ((row - 1) * num_cols) + (col - 1));
      center = *(floor + ((row - 1) * num_cols) + col);
      if (col == num_cols - 1)
        right = '.';
      else
        right = *(floor + ((row - 1) * num_cols) + (col + 1));
//printf("%c%c%c => ", left, center, right);
      if ((left == '^' && center == '^' && right == '.') ||
          (left == '.' && center == '^' && right == '^') ||
          (left == '^' && center == '.' && right == '.') ||
          (left == '.' && center == '.' && right == '^')) {
        *(floor + (row * num_cols) + col) = '^';
//printf("^\n");
      } else {
        *(floor + (row * num_cols) + col) = '.';
//printf(".\n");
        num_safe_spaces++;
      }
    }
//printf("\n");
  }

  display_floor(floor, num_rows, num_cols);
  printf("Number of safe spaces: %d\n", num_safe_spaces);

  return 0;
}
