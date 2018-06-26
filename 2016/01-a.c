#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define GRIDSIZE  1000

int grid[GRIDSIZE][GRIDSIZE] = { { 0 } };
int translation_size = GRIDSIZE / 2;

int visit(int x, int y) {
  int tmp_x = x + translation_size;
  int tmp_y = y + translation_size;

  grid[tmp_x][tmp_y]++;
  return grid[tmp_x][tmp_y];
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  int moves;
  
  int x;
  int y;
  int heading;
  int distance;

  char turn;
  int steps;

  int *h;
  int step;
  int twice_found;
  int twice_x;
  int twice_y;
  int v_count;

  if (argc < 2)
    filename = "01-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  x = 0;
  y = 0;
  heading = 0;
  twice_found = 0;
  (void)visit(x, y);

  moves = 0;

printf("%d (%d, %d)\n", heading, x, y);
  while (1) {
    ret = fscanf(fp, "%c", &turn);
    if ((ret == EOF) || (turn == '\n')) {
      break;
    } else if (ret != 1) {
      printf("Error: Expected a turn, scanned %d instead\n", ret);
      return 1;
    }
    if (turn == 'R') {
      heading++;
      if (heading > 3)
         heading -= 4;
    } else if (turn == 'L') {
      heading--;
      if (heading < 0)
         heading += 4;
    } else {
      printf("Error: Unrecognized turn '%c'\n", turn);
      return 1;
    }

    ret = fscanf(fp, "%d", &steps);
    if (ret == EOF) {
      printf("Error: Expected steps, scanned EOF instead\n");
      return 1;
    } else if (ret != 1) {
      printf("Error: Expected steps, scanned %d instead\n", ret);
      return 1;
    }

    moves++;
printf("%d: %c%d: ", moves, turn, steps);

    switch (heading) {
      case 0: h = &y; step = 1; break;
      case 1: h = &x; step = 1; break;
      case 2: h = &y; step = -1; break;
      case 3: h = &x; step = -1; break;
    }
    while (steps) {
      *h += step;
      v_count = visit(x, y);
      if (!twice_found && v_count == 2) {
        twice_x = x;
        twice_y = y;
        twice_found = 1;
      }
      steps--;
    }
printf("%d (%d, %d)\n", heading, x, y);

    ret = fscanf(fp, ", ");
    if (ret == EOF) {
      printf("Error: Expected delimiter, scanned EOF instead\n");
      return 1;
    }
  }

  distance = abs(x) + abs(y);
  printf("%d blocks\n", distance);
  if (twice_found) {
    printf("First intersection visited twice: (%d, %d)\n", twice_x, twice_y);
    distance = abs(twice_x) + abs(twice_y);
    printf("%d blocks\n", distance);
  } else {
    printf("No intersection was visited twice\n");
  }

  return 0;
}
