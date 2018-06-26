#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define SCREEN_X_SIZE  50
#define SCREEN_Y_SIZE  6

#define LINE_SIZE  128
#define LINE_SIZE_S  "128"

static int screen[SCREEN_X_SIZE][SCREEN_Y_SIZE] = { { 0 } };
static int tmp_row[SCREEN_X_SIZE];
static int tmp_col[SCREEN_Y_SIZE];

static void print_screen(int show_blanks) {
  int x, y;
  char *b;

  if (show_blanks)
    b = ".";
  else
    b = " ";

  for (y = 0; y < SCREEN_Y_SIZE; y++) {
    for (x = 0; x < SCREEN_X_SIZE; x++) {
      printf("%s", (screen[x][y] ? "#" : b));
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
  char *p;
  int x, y;
  int i, j;
  int num_lit;

  if (argc < 2)
    filename = "08-a-input";
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

    p = line;
    if (!strncmp(p, "rect ", 5)) {
      p += 5;
      x = atoi(p);
      do { p++; } while (*p != 'x'); p++;
      y = atoi(p);
//printf("rect(%d,%d) [%s]\n", x, y, line);
      for (i = 0; i < x; i++)
        for (j = 0; j < y; j++)
          screen[i][j] = 1;
    } else if (!strncmp(p, "rotate column x=", 16)) {
      p += 16;
      x = atoi(p);
      do { p++; } while (*p != ' '); p++;
      do { p++; } while (*p != ' '); p++;
      y = atoi(p);
//printf("rcol(%d,%d) [%s]\n", x, y, line);
      for (i = 0; i < SCREEN_Y_SIZE; i++)
        tmp_col[i] = screen[x][i];
      for (i = 0; i < SCREEN_Y_SIZE; i++)
        screen[x][i] = tmp_col[(i - y + SCREEN_Y_SIZE) % SCREEN_Y_SIZE];
    } else if (!strncmp(p, "rotate row y=", 13)) {
      p += 13;
      y = atoi(p);
      do { p++; } while (*p != ' '); p++;
      do { p++; } while (*p != ' '); p++;
      x = atoi(p);
//printf("rrow(%d,%d) [%s]\n", y, x, line);
      for (i = 0; i < SCREEN_X_SIZE; i++)
        tmp_row[i] = screen[i][y];
      for (i = 0; i < SCREEN_X_SIZE; i++)
        screen[i][y] = tmp_row[(i - x + SCREEN_X_SIZE) % SCREEN_X_SIZE];
    } else {
      printf("Error: Unhandled command '%s'\n", line);
      return 1;
    }
  }

  print_screen(0);
  num_lit = 0;
  for (i = 0; i < SCREEN_X_SIZE; i++)
    for (j = 0; j < SCREEN_Y_SIZE; j++)
      if (screen[i][j])
        num_lit++;
  printf("Number of lit pixels: %d\n", num_lit);

  return 0;
}
