#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char key[128] = { 0 };
  char *p;
  int i;
  unsigned char *md5hash;
  int key_len;
  int num_len;
  int found_5;
  int found_6;

  if (argc < 2)
    filename = "04-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  ret = fscanf(fp, "%s\n", key);
  if ((ret == EOF) || (ret != 1)) {
    printf("Error: Expected room, scanned %d instead\n", ret);
    return 1;
  }

printf(":%s:\n", key);
  key_len = strlen(key);
  p = key + key_len;
  i = 1;
  found_5 = 0;
  found_6 = 0;
  while (1) {
    num_len = sprintf(p, "%d", i);
    md5hash = MD5((unsigned char *)key, key_len + num_len, NULL);
    if (!found_5 && md5hash[0] == 0x00 && md5hash[1] == 0x00 && md5hash[2] < 0x10) {
      printf("5 zeros: %d\n", i);
      found_5 = 1;
    } else if (!found_6 && md5hash[0] == 0x00 && md5hash[1] == 0x00 && md5hash[2] == 0x00) {
      printf("6 zeros: %d\n", i);
      found_6 = 1;
    }
    if (found_5 && found_6)
      break;
    i++;
  }

  return 0;
}
