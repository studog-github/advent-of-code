#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

#define DEFAULT_INPUT "20-a-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

struct fragment {
  unsigned long start;
  unsigned long end;
  struct list_head list;
};

#if 0
static inline void print_list(struct list_head *list) {
  struct list_head *p;
  struct fragment *f;
  struct fragment *l;
  bool first;

  first = true;
  list_for_each(p, list) {
    f = list_entry(p, struct fragment, list);
    if (first) {
      first = false;
    } else {
      if (l->end >= f->start - 1)
        printf("^");
      else
        printf(",");
    }
    printf("[%10lu-%10lu]", f->start, f->end);
    l = f;
  }
  printf("\n");
}
#endif

static inline void insert_fragment(struct fragment *new, struct list_head *list)
{
  struct list_head *p;
  struct fragment *f;
  struct fragment *next;

  list_for_each(p, list) {
    f = list_entry(p, struct fragment, list);
    if ((new->end < f->start) && (f->start - new->end > 1)) {
      list_add_tail(&new->list, p);
      return;
    } else if ((new->start > f->end) && (new->start - f->end > 1)) {
      continue;
    }
    // merge
    f->start = MIN(f->start, new->start);
    f->end = MAX(f->end, new->end);
    free(new);

    while (f->list.next != list) {
      next = list_next_entry(f, list);
      if ((next->start > f->end) && (next->start - f->end > 1))
        break;
      f->end = MAX(f->end, next->end);
      list_del(&next->list);
      free(next);
    }

    return;
  }
  list_add_tail(&new->list, list);
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  unsigned long lower;
  unsigned long upper;

  struct list_head fragments;
  struct fragment *tmpfrag;

  unsigned long num_denied;
  struct list_head *p;

  if (argc < 2)
    filename = DEFAULT_INPUT;
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  INIT_LIST_HEAD(&fragments);

  // first line is valid range
  ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
  if (ret == EOF)
    return 0;
  if (ret != 1) {
    printf("Error: Expected 1, scanned %d instead\n", ret);
    return 1;
  }

  i = 0;
  lower = atol(&line[i]);
  do { i++; } while (line[i] != '-'); i++;
  upper = atol(&line[i]);

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    tmpfrag = malloc(sizeof(*tmpfrag));
    if (tmpfrag == NULL) {
      printf("Error: Out of memory (fragment)\n");
      return 1;
    }

    i = 0;
    tmpfrag->start = atol(&line[i]);
    do { i++; } while (line[i] != '-'); i++;
    tmpfrag->end = atol(&line[i]);

    insert_fragment(tmpfrag, &fragments);
  }

  tmpfrag = list_first_entry_or_null(&fragments, struct fragment, list);
  if (tmpfrag == NULL || lower < tmpfrag->start) {
    printf("Lowest allowed IP is %lu\n", lower);
  } else if (upper > tmpfrag->end) {
    printf("Lowest allowed IP is %lu\n", tmpfrag->end + 1);
  } else {
    printf("All IPs are disallowed\n");
  }

  num_denied = 0;
  if (!list_empty(&fragments)) {
    list_for_each(p, &fragments) {
      tmpfrag = list_entry(p, struct fragment, list);
      num_denied += tmpfrag->end - tmpfrag->start + 1;
    }
  }
  printf("Number of IPs allowed is %lu\n", (upper - lower + 1) - num_denied);

  return 0;
}
