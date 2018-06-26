#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void turn_on(int *light) {
  (*light)++;
}
void turn_off(int *light) {
  (*light)--;
  if (*light < 0)
    *light = 0;
}
void toggle(int *light) {
  *light += 2;
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int lights[1000][1000] = { { 0 } };
  char cmd[16] = { 0 };
  int sx, sy, ex, ey, x, y;
  void (*op)(int *);

  int brightness;

  if (argc < 2)
    filename = "06-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  while (1) {
    ret = fscanf(fp, "%s", cmd);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }
    if (!strncmp(cmd, "turn", 4)) {
      ret = fscanf(fp, "%s", cmd);
      if (ret == EOF) {
        printf("Error: Unexpected EOF after 'turn'\n");
        return 1;
      }
      if (ret != 1) {
        printf("Error: Expected 1, scanned %d instead\n", ret);
        return 1;
      }
    }
    ret = fscanf(fp, " %d,%d through %d,%d\n", &sx, &sy, &ex, &ey);
    if (ret == EOF) {
      printf("Error: Unexpected EOF after cmd\n");
      return 1;
    }
    if (ret != 4) {
      printf("Error: Expected 4, scanned %d instead\n", ret);
      return 1;
    }

    // This assumption was implied but not stated in the requirements
    if ((sx >= ex && sy >= ey) && !(sx == ex && sy == ey)) {
      printf("Error: Bad rect found (%d, %d) => (%d, %d)\n", sx, sy, ex, ey);
      return 1;
    }

    if (!strncmp(cmd, "on", 2)) {
      op = turn_on;
    } else if (!strncmp(cmd, "off", 3)) {
      op = turn_off;
    } else if (!strncmp(cmd, "toggle", 6)) {
      op = toggle;
    } else {
      printf("Error: Unrecognized command '%s'\n", cmd);
      return 1;
    }

    // traverse all lights in rect
    for (x = sx; x <= ex; x++) {
      for (y = sy; y <= ey; y++) {
        op(&lights[x][y]);
      }
    }
  }

  brightness = 0;
  for (x = 0; x < 1000; x++)
    for (y = 0; y < 1000; y++)
      if (lights[x][y])
        brightness += lights[x][y];

  printf("Brightness: %d\n", brightness);

  return 0;
}
