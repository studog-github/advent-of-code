#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define GRID_SIZE  5000

int houses[GRID_SIZE][GRID_SIZE] = { { 0 } };

int visit(int x, int y) {
  int tmp_x = x + (GRID_SIZE / 2);
  int tmp_y = y + (GRID_SIZE / 2);

  houses[tmp_x][tmp_y]++;

  return houses[tmp_x][tmp_y];
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char move;
  int x;
  int y;
  int num_houses;

  if (argc < 2)
    filename = "03-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  x = 0;
  y = 0;
  visit(x, y);
  num_houses = 1;
  while (1) {
    ret = fscanf(fp, "%c\n", &move);
    if ((ret == EOF) || (move == '\n')) {
      break;
    } else if (ret != 1) {
      printf("Error: Expected room, scanned %d instead\n", ret);
      return 1;
    }
    if (move == '^')
      y++;
    else if (move == '>')
      x++;
    else if (move == 'v')
      y--;
    else if (move == '<')
      x--;
    else {
      printf("Error: Unrecognized move '%c' 0x%02x\n", move, move);
      return 1;
    }
    if (visit(x, y) == 1)
      num_houses++;
printf("m: %c %02x  (%d, %d)  #: %d\n", move, move, x, y, num_houses);
  }

  printf("Houses: %d\n", num_houses);

  return 0;
}
