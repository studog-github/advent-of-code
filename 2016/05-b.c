#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

#define DOOR_ID_SIZE  1024

void dump_md5(unsigned char *hash) {
  int i;

  printf("md5:");
  for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    printf(" %02x", hash[i]);
  printf("\n");
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char door_id[DOOR_ID_SIZE];
  int door_id_len;
  int remain_len;
  char *door_id_ptr;

  int pass_set[8];
  char password[8];
  int pw_idx;

  int i;
  unsigned char *md5hash;
  int pos;

  if (argc < 2)
    filename = "05-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  ret = fscanf(fp, "%s\n", door_id);
  if ((ret == EOF) || (ret != 1)) {
    printf("Error: Expected room, scanned %d instead\n", ret);
    return 1;
  }

  memset(pass_set, 0, sizeof(int) * 8);

  door_id_len = strlen(door_id);
  door_id_ptr = door_id + door_id_len;
  remain_len = DOOR_ID_SIZE - door_id_len;

  i = 0;
  for (pw_idx = 0; pw_idx < 8; pw_idx++) {
    while (1) {
      ret = snprintf(door_id_ptr, remain_len, "%d", i);
      md5hash = MD5((unsigned char *)door_id, door_id_len + ret, NULL);
      i++;
      pos = md5hash[2] & 0x0f;
      if (md5hash[0] == 0x00 && md5hash[1] == 0x00 && md5hash[2] < 0x08 &&
          pass_set[pos] == 0)
        break;
    }
dump_md5(md5hash);
    password[pos] = (md5hash[3] & 0xf0) >> 4;
    pass_set[pos] = 1;
  }

  for (pw_idx = 0; pw_idx < 8; pw_idx++) {
    printf("%x", password[pw_idx]);
  }
  printf("\n");

  return 0;
}
