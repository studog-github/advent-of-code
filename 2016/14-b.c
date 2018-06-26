#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <openssl/md5.h>
#include "list.h"

#define MD5_DIGEST_LENGTH_A  (MD5_DIGEST_LENGTH * 2)

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

static inline int ahdtoi(char ahd)
{
  if (ahd >= '0' && ahd <= '9')
    return ahd - '0';
  if (ahd >= 'a' && ahd <= 'f')
    return ahd - 'a';
  if (ahd >= 'A' && ahd <= 'F')
    return ahd - 'A';
  return -1;
}

static void md5tostr(unsigned char *md5, char *buf)
{
  int i;
  char *p;

  for (i = 0, p = buf; i < MD5_DIGEST_LENGTH; i++)
    p += sprintf(p, "%02x", md5[i]);
}

#if 0
static void dump_md5(unsigned char *hash)
{
  int i;

  printf("md5:");
  for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    printf(" %02x", hash[i]);
  printf("\n");
}
#endif

#if 0
static void dump_md5(unsigned char *hash)
{
  char str[MD5_DIGEST_LENGTH_A + 1];

  md5tostr(hash, str);
  printf("md5: %s\n", str);
}
#endif

typedef struct _santa_key_t {
  int idx;
  char md5str[MD5_DIGEST_LENGTH_A + 1];
  int triple;
  int rep5;
  struct list_head list;
} santa_key_t;

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };

  int salt_len;
  char *p;
  int i;
  int num_keys;
  unsigned char *hash;
  santa_key_t *newkey;
  int j;
  struct list_head *potkeyptr;
  struct list_head *tmpkeyptr;
  santa_key_t *potkey;
  int ahd;

  struct list_head potential_keys;
  struct list_head keys;
  bool window_checked_for[16];

  if (argc < 2)
    filename = "14-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }
  }

  INIT_LIST_HEAD(&potential_keys);
  INIT_LIST_HEAD(&keys);

  salt_len = strlen(line);
  p = line + salt_len;

  for (i = 0, num_keys = 0; /**/; i++) {
printf("%d  %d\n", num_keys, i);
    list_for_each_safe(potkeyptr, tmpkeyptr, &potential_keys) {
      potkey = list_entry(potkeyptr, santa_key_t, list);
//printf("DEL/KEY? [%d] [%s] [%d] [%d] ", potkey->idx, potkey->md5str, potkey->triple, potkey->rep5);
      if (potkey->rep5 == -1) {
        if (potkey->idx < i - 1000) {
          list_del(potkeyptr);
          free(potkey);
//printf("DEL\n");
        } else {
//printf("\n");
          break;
        }
      } else {
        list_del(potkeyptr);
        list_add(potkeyptr, &keys);
        num_keys++;
//printf("KEY %d\n", num_keys);
        if (num_keys == 64)
          goto found_64_keys;
      }
    }

    ret = sprintf(p, "%d", i);
    hash = MD5((unsigned char *)line, salt_len + ret, NULL);

    newkey = malloc(sizeof(*newkey));
    if (newkey == NULL) {
      printf("Error: Out of memory (window)\n");
      return 1;
    }
    newkey->idx = i;
    md5tostr(hash, newkey->md5str);
    for (j = 0; j < 2016; j++) {
      hash = MD5((unsigned char *)newkey->md5str, MD5_DIGEST_LENGTH_A, NULL);
      md5tostr(hash, newkey->md5str);
    }
    newkey->triple = -1;
    newkey->rep5 = -1;

    for (j = 0; j < MD5_DIGEST_LENGTH_A - 2; /**/) {
      if (newkey->md5str[j] != newkey->md5str[j + 1]) {
        j += 1;
      } else if (newkey->md5str[j + 1] != newkey->md5str[j + 2]) {
        j += 2;
      } else {
        newkey->triple = j;
        break;
      }
    }
//printf("[%s] [%s] [%d] [%d]\n", line, newkey->md5str, newkey->triple, newkey->rep5);

    for (j = 0; j < 16; j++)
      window_checked_for[j] = false;

    for (j = 0; j < MD5_DIGEST_LENGTH_A - 4; /**/) {
      ahd = ahdtoi(newkey->md5str[j]);
      if (window_checked_for[ahd] || (newkey->md5str[j] != newkey->md5str[j + 1])) {
        j += 1;
      } else if (newkey->md5str[j + 1] != newkey->md5str[j + 2]) {
        j += 2;
      } else if (newkey->md5str[j + 2] != newkey->md5str[j + 3]) {
        j += 3;
      } else if (newkey->md5str[j + 3] != newkey->md5str[j + 4]) {
        j += 4;
      } else {
//printf("5REP %d:%c\n", j, newkey->md5str[j]);
        list_for_each_safe(potkeyptr, tmpkeyptr, &potential_keys) {
          potkey = list_entry(potkeyptr, santa_key_t, list);
//printf("CONSIDER [%d] [%s] [%d] [%d] [%c:%c] ", potkey->idx, potkey->md5str, potkey->triple, potkey->rep5, potkey->md5str[potkey->triple], newkey->md5str[j]);
          if (potkey->rep5 == -1 && potkey->md5str[potkey->triple] == newkey->md5str[j]) {
            potkey->rep5 = i;
//printf("KEY");
            if (potkeyptr == potential_keys.next) {
              list_del(potkeyptr);
              list_add(potkeyptr, &keys);
              num_keys++;
//printf(" %d", num_keys);
              if (num_keys == 64)
                goto found_64_keys;
            }
          }
//printf("\n");
        }
        window_checked_for[ahd] = true;
        j += 5;
      }
    }

    if (newkey->triple == -1) {
      free(newkey);
    } else {
      list_add_tail(&newkey->list, &potential_keys);
    }
  }

found_64_keys:
  printf("Index: %d\n", potkey->idx);

  return 0;
}
