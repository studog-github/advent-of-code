#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a, b) ({           \
          int _a = (a);        \
          int _b = (b);        \
          _a <= _b ? _a : _b;  \
        })

#define MAX(a, b) ({           \
          int _a = (a);        \
          int _b = (b);        \
          _a >= _b ? _a : _b;  \
        })

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int a;
  int b;
  int c;
  int length;
  int width;
  int height;
  int area_s1;
  int area_s2;
  int area_s3;

  int paper_sum;
  int ribbon_sum;

  if (argc < 2)
    filename = "02-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  paper_sum = 0;
  ribbon_sum = 0;
  while (1) {
    ret = fscanf(fp, "%dx%dx%d\n", &a, &b, &c);
    if (ret == EOF) {
      break;
    } else if (ret != 3) {
      printf("Error: Expected room, scanned %d instead\n", ret);
      return 1;
    }
    length = MIN(a, MIN(b, c));
    if (length == a) {
      width = MIN(b, c);
      height = MAX(b, c);
    } else if (length == b) {
      width = MIN(a, c);
      height = MAX(a, c);
    } else {
      width = MIN(a, b);
      height = MAX(a, b);
    }
    area_s1 = length * width;
    area_s2 = length * height;
    area_s3 = width * height;
    paper_sum += 2 * (area_s1 + area_s2 + area_s3) + area_s1;
    ribbon_sum += 2 * (length + width) + (length * width * height);
  }

  printf("Paper needed: %d\n", paper_sum);
  printf("Ribbon needed: %d\n", ribbon_sum);

  return 0;
}
