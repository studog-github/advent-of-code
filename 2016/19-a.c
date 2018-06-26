#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

typedef struct _elf_t {
  int idx;
  int presents;
  struct list_head circle;
} elf_t;

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };

  int num_elves;
  int i;
  elf_t *tmpelf;
  elf_t *curelf;
  //elf_t *curelf;
  //struct list_head *tptr;
  //struct list_head *tlptr;

  if (argc < 2)
    filename = "19-a-input";
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

//printf(":%s:\n", line);
  }

  num_elves = atoi(line);

  tmpelf = malloc(sizeof(*tmpelf));
  if (tmpelf == NULL) {
    printf("Error: Out of memory (first elf)\n");
    return 1;
  }
  tmpelf->idx = 1;
  tmpelf->presents = 1;
  INIT_LIST_HEAD(&tmpelf->circle);

  for (i = 1, curelf = tmpelf; i < num_elves; i++, curelf = tmpelf) {
    tmpelf = malloc(sizeof(*tmpelf));
    if (tmpelf == NULL) {
      printf("Error: Out of memory (elf)\n");
      return 1;
    }
    tmpelf->idx = i + 1;
    tmpelf->presents = 1;
    list_add(&tmpelf->circle, &curelf->circle);
  }

  curelf = list_entry(curelf->circle.next, elf_t, circle);
  tmpelf = list_entry(curelf->circle.next, elf_t, circle);
  while (curelf != tmpelf) {
    curelf->presents += tmpelf->presents;
//printf("Elf %d takes Elf %d's %d presents and has %d presents now\n", curelf->idx,
//       tmpelf->idx, tmpelf->presents, curelf->presents);
    list_del(&tmpelf->circle);
    free(tmpelf);
    curelf = list_entry(curelf->circle.next, elf_t, circle);
    tmpelf = list_entry(curelf->circle.next, elf_t, circle);
  }

  printf("Elf %d has %d presents\n", curelf->idx, curelf->presents);

  return 0;
}
