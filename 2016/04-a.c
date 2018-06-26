#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define DUMP_ARRAYS  ({                           \
          int a;                                  \
          printf("letters:");                     \
          for (a = 0; a < 26; a++)                \
            printf("  %c", a + 'a');              \
          printf("\n");                           \
          printf("abcount:");                     \
          for (a = 0; a < 26; a++)                \
            if (abcount[a].count == 0)            \
              printf(" --");                      \
            else                                  \
              printf(" %02d", abcount[a].count);  \
          printf("\n");                           \
          printf("        ");                     \
          for (a = 0; a < 26; a++)                \
            printf(" %02d", abcount[a].sortidx);  \
          printf("\n");                           \
          printf(" absort:");                     \
          for (a = 0; a < 26; a++)                \
            printf("  %c", absort[a] + 'a');      \
          printf("\n");                           \
          printf("\n");                           \
        })

struct abdata {
  int count;
  int sortidx;
};

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char room_encrypted[1024];
  char *p;
  int i;
  int current_letter;
  int next_sortidx;
  int swap_letter;
  struct abdata abcount[26];
  int absort[26];
  int sectorid;
  int sectorid_sum;

  if (argc < 2)
    filename = "04-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  sectorid_sum = 0;
  while (1) {
    ret = fscanf(fp, "%s\n", room_encrypted);
    if (ret == EOF) {
      break;
    } else if (ret != 1) {
      printf("Error: Expected room, scanned %d instead\n", ret);
      return 1;
    }
//printf("room[%s]\n", room_encrypted);
    p = room_encrypted;
    for (i = 0; i < 26; i++) {
      abcount[i].count = 0;
      abcount[i].sortidx = i;
      absort[i] = i;
    }
//DUMP_ARRAYS;
    while (1) {
      if (*p == '-') {
        // Skip
      } else if ((*p >= 'a') && (*p <= 'z')) {
        current_letter = *p - 'a';
        abcount[current_letter].count++;
        next_sortidx = abcount[current_letter].sortidx - 1;
//printf("cur_let: %c %d\n", *p, current_letter);
//DUMP_ARRAYS;
        while (next_sortidx >= 0) {
          swap_letter = absort[next_sortidx];
//printf("next_sort: %d  swap_let: %c %d\n", next_sortidx, swap_letter + 'a', swap_letter);
          if ((abcount[swap_letter].count > abcount[current_letter].count) ||
              ((abcount[swap_letter].count == abcount[current_letter].count) && (swap_letter < current_letter))) {
//printf("break\n\n");
            break;
          }
          absort[next_sortidx] = current_letter;
          absort[next_sortidx + 1] = swap_letter;
          abcount[current_letter].sortidx--;
          abcount[swap_letter].sortidx++;
//DUMP_ARRAYS;
          next_sortidx--;
        }
      } else if ((*p >= '0') && (*p <= '9')) {
        break;
      } else {
        printf("Error: Unexpected character '%c' 0x%02x\n", *p, *p);
        return 1;
      }
      p++;
    }
    sscanf(p, "%d", &sectorid);
    while (*p != '[')
      p++;
    for (p++, i = 0; (*p - 'a') == absort[i]; p++, i++)
      ;
    if (*p != ']')
      continue;
    sectorid_sum += sectorid;
    p = room_encrypted;
    while (1) {
      if (*p == '-') {
        *p = ' ';
      } else if ((*p >= 'a') && (*p <= 'z')) {
        *p = (((*p - 'a') + sectorid) % 26) + 'a';
      } else {
        break;
      }
      p++;
    }
    p--;
    *p = 0;
    printf("%s %d\n", room_encrypted, sectorid);
  }

  printf("Sector ID sum: %d\n", sectorid_sum);

  return 0;
}
