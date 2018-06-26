#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char c;
  int pos;
  int floor;
  int basement_found;
  int basement_pos;

  if (argc < 2)
    filename = "01-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  pos = 0;
  floor = 0;
  basement_found = 0;

  while (1) {
    ret = fscanf(fp, "%c", &c);
    if (ret == EOF) {
      printf("Error: Unexpected EOF\n");
      return 1;
    } else if (ret != 1) {
      printf("Error: Expected room, scanned %d instead\n", ret);
      return 1;
    }
    pos++;
    if (c == '(')
      floor++;
    else if (c == ')')
      floor--;
    else if (c == '\n')
      break;
    else {
      printf("Error: Unrecognized input '%c' 0x%02x\n", c, c);
      return 1;
    }
    if (!basement_found && floor < 0) {
      basement_pos = pos;
      basement_found = 1;
    }
  }

  printf("Floor %d\n", floor);
  printf("Basement entered at %d\n", basement_pos);

  return 0;
}
