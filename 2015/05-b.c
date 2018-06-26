#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int nice;
  char str[128] = { 0 };
  int str_len;

  int i;
  int j;
  int pair_found;
  int tri_found;

  if (argc < 2)
    filename = "05-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  nice = 0;
  while (1) {
    ret = fscanf(fp, "%s\n", str);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected room, scanned %d instead\n", ret);
      return 1;
    }

    str_len = strlen(str);

    pair_found = 0;
    for (i = 0; i < str_len - 3 && !pair_found; i++)
      for (j = i + 2; j < str_len - 1 && !pair_found; j++)
        if (!strncmp(&str[i], &str[j], 2))
          pair_found = 1;

    tri_found = 0;
    for (i = 0; i < str_len - 2 && !tri_found; i++)
      if (str[i] == str[i + 2])
        tri_found = 1;

    if (pair_found && tri_found)
      nice++;
  }

  printf("Nice strings: %d\n", nice);

  return 0;
}
