#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <values.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char message[128][26] = { { 0 } };
  char garbled[128] = { 0 };
  int i;
  int j;
  int count;

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
    ret = fscanf(fp, "%s\n", garbled);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected line, scanned %d instead\n", ret);
      return 1;
    }
    for (i = 0; garbled[i]; i++) {
      if (garbled[i] < 'a' || garbled[i] > 'z') {
        printf("Error: Unrecognized character '%c' 0x%02x\n", garbled[i], garbled[i]);
        return 1;
      }
      message[i][garbled[i] - 'a']++;
    }
  }

#if 0
printf("   ");
for (j = 0; j < 26; j++)
  printf("  %c", j + 'a');
printf("\n");

for (i = 0; garbled[i]; i++) {
  printf("%2d:", i);
  for (j = 0; j < 26; j++) {
    if (message[i][j])
      printf(" %2d", message[i][j]);
    else
      printf("  -");
  }
  printf("\n");
}
#endif

  for (i = 0; garbled[i]; i++) {
    for (j = 0, count = 0; j < 26; j++) {
      if (message[i][j] > count) {
        garbled[i] = j + 'a';
        count = message[i][j];
      }
    }
  }
  printf("Most: %s\n", garbled);

  for (i = 0; garbled[i]; i++) {
    for (j = 0, count = INT_MAX; j < 26; j++) {
      if (message[i][j] && message[i][j] < count) {
        garbled[i] = j + 'a';
        count = message[i][j];
      }
    }
  }
  printf("Least: %s\n", garbled);

  return 0;
}
