#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char str[128] = { 0 };
  int str_len;
  int nice;
  int i;
  int num_vowels;
  int double_found;

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
    str_len--;
    num_vowels = 0;
    double_found = 0;
    for (i = 0; i < str_len; i++) {
      if (!strncmp(&str[i], "ab", 2) || !strncmp(&str[i], "cd", 2) ||
          !strncmp(&str[i], "pq", 2) || !strncmp(&str[i], "xy", 2))
        break;
      if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] == 'o' || str[i] == 'u')
        num_vowels++;
      if (str[i] == str[i + 1])
        double_found = 1;
    }
    if (i == str_len) {
      if (str[i] == 'a' || str[i] == 'e' || str[i] == 'i' || str[i] == 'o' || str[i] == 'u')
        num_vowels++;
      if (num_vowels >= 3 && double_found)
        nice++;
    }
  }

  printf("Nice strings: %d\n", nice);

  return 0;
}
