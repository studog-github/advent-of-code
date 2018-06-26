#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define DEFAULT_INPUT "20-a-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  unsigned long lower;
  unsigned long upper;

  unsigned char *ips;

  unsigned long start;
  unsigned long end;
  unsigned long j;
  unsigned long num_allowed;

  if (argc < 2)
    filename = DEFAULT_INPUT;
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  // first line is valid range
  ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
  if (ret == EOF)
    return 0;
  if (ret != 1) {
    printf("Error: Expected 1, scanned %d instead\n", ret);
    return 1;
  }

  i = 0;
  lower = atol(&line[i]);
  do { i++; } while (line[i] != '-'); i++;
  upper = atol(&line[i]);

  ips = malloc((upper - lower + 1) * sizeof(*ips));
  if (ips == NULL) {
    printf("Error: Out of memory (ips)\n");
    return 1;
  }
  memset(ips, 0, (upper - lower + 1) * sizeof(*ips));

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

//printf(":%s:\n", line);
    i = 0;
    start = atol(&line[i]);
    do { i++; } while (line[i] != '-'); i++;
    end = atol(&line[i]);

    start -= lower;
    end -= lower;

    for (j = start; j <= end; j++)
      ips[j] = 1;
  }

  for (j = 0; j <= upper - lower; j++)
    if (ips[j] == 0)
      break;
  printf("Lowest allowed IP is %lu\n", j + lower);

  num_allowed = 0;
  for (j = 0; j <= upper - lower; j++)
    if (ips[j] == 0)
      num_allowed++;
  printf("Number of IPs allowed is %lu\n", num_allowed);

  return 0;
}
