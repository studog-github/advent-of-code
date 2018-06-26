#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int num_ips;

  char addr[1024] = { 0 };
  int i;
  int window_size;
  int abba_found;
  int hypernet;

  if (argc < 2)
    filename = "07-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  num_ips = 0;
  while (1) {
    ret = fscanf(fp, "%s\n", addr);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    window_size = 0;
    abba_found = 0;
    hypernet = 0;
    for (i = 0; addr[i]; i++) {
      if (addr[i] == '[') {
        window_size = 0;
        hypernet = 1;
      } else if (addr[i] == ']') {
        window_size = 0;
        hypernet = 0;
      } else if (addr[i] < 'a' || addr[i] > 'z') {
        printf("Error: Unrecognized character '%c' 0x%02x\n", addr[i], addr[i]);
        return 1;
      } else if (window_size < 3) {
        window_size++;
      } else if (addr[i] == addr[i - 3] && addr[i - 1] == addr[i - 2] && addr[i] != addr[i - 1]) {
        if (hypernet)
          break;
        abba_found = 1;
      }
    }
    if (!addr[i] && abba_found)
      num_ips++;
  }

  printf("IPs that support TLS: %d\n", num_ips);

  return 0;
}
