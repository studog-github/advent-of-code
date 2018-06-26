#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef struct _str_t {
  char str[3];
  struct _str_t *next;
} str_t;

void dump_list(char *name, str_t *list) {
  str_t *p;
  int num;

  printf("%s:", name);
  for (p = list, num = 0; p; p = p->next, num++)
    printf(" %c%c%c", p->str[0], p->str[1], p->str[2]);
  printf(" (%d)\n", num);
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  int num_ips;

  char addr[1024] = { 0 };
  int i;
  int window_size;
  int hypernet;
  str_t *aba;
  str_t *bab;
  str_t *lp;

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
  aba = NULL;
  bab = NULL;

  while (1) {
    ret = fscanf(fp, "%s\n", addr);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    window_size = 0;
    hypernet = 0;
    for (i = 0; addr[i]; i++) {
//printf("w:%d h:%d %d: %c", window_size, hypernet, i, addr[i]);
      if (addr[i] == '[') {
//printf(" [\n");
        window_size = 0;
        hypernet = 1;
      } else if (addr[i] == ']') {
//printf(" ]\n");
        window_size = 0;
        hypernet = 0;
      } else if (addr[i] < 'a' || addr[i] > 'z') {
        printf("Error: Unrecognized character '%c' 0x%02x\n", addr[i], addr[i]);
        return 1;
      } else if (window_size < 2) {
//printf(" window\n");
        window_size++;
      } else if (addr[i] == addr[i - 2] && addr[i] != addr[i - 1]) {
//printf(" %s\n", (hypernet ? "bab" : "aba"));
        for (lp = (hypernet ? aba : bab); lp; lp = lp->next) {
          if (addr[i] == lp->str[1] && addr[i - 1] == lp->str[0]) {
            num_ips++;
            break;
          }
        }
        if (lp) {
          break;
        } else {
          lp = malloc(sizeof(str_t));
          memcpy(lp->str, &addr[i - 2], 3);
          if (hypernet) {
            lp->next = bab;
            bab = lp;
          } else {
            lp->next = aba;
            aba = lp;
          }
//dump_list("aba", aba);
//dump_list("bab", bab);
        }
      }
//else {
//printf("\n");
//}
    }

    while (aba) {
      lp = aba->next;
      free(aba);
      aba = lp;
    }
    while (bab) {
      lp = bab->next;
      free(bab);
      bab = lp;
    }
  }

  printf("IPs that support TLS: %d\n", num_ips);

  return 0;
}
