#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char move;

  char keypad[7][7] = {
                        { '-', '-', '-', '-', '-', '-', '-' },
                        { '-', '-', '-', '5', '-', '-', '-' },
                        { '-', '-', '2', '6', 'A', '-', '-' },
                        { '-', '1', '3', '7', 'B', 'D', '-' },
                        { '-', '-', '4', '8', 'C', '-', '-' },
                        { '-', '-', '-', '9', '-', '-', '-' },
                        { '-', '-', '-', '-', '-', '-', '-' },
                      };
  int x;
  int y;

  char code[1024] = { 0 };
  int idx;

  if (argc < 2)
    filename = "02-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  idx = 0;
  // Start on 5
  x = 1;
  y = 3;

//printf("Button: %c\n", keypad[x][y]);
  while (1) {
    ret = fscanf(fp, "%c", &move);
    if (ret == EOF) {
      break;
    } else if (ret != 1) {
      printf("Error: Expected a move, scanned %d %02x instead\n", ret, move);
      return 1;
    }

    if (move == '\n') {
      code[idx] = keypad[x][y];
//printf("Outputting: %c\n", code[idx]);
      idx++;
    } else if (move == 'U') {
      y--; if (keypad[x][y] == '-') y++;
    } else if (move == 'D') {
      y++; if (keypad[x][y] == '-') y--;
    } else if (move == 'L') {
      x--; if (keypad[x][y] == '-') x++;
    } else if (move == 'R') {
      x++; if (keypad[x][y] == '-') x--;
    } else {
      printf("Error: Unrecognized move '%c' %02x\n", move, move);
      return 1;
    }
  }

  printf("Code [%s]\n", code);

  return 0;
}
