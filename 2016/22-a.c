#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

#define SKIP_TO(c) ({                                             \
          do { i++; } while (line[i] != c && line[i] != '\0');    \
          if (line[i] == '\0') {                                  \
            printf("Error: Unexpected EOL '%s'\n", line);         \
            return 1;                                             \
          }                                                       \
        })
#define SKIP_OVER(c) ({     \
          SKIP_TO(c); i++;  \
        })

#define DEFAULT_INPUT "22-a-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

typedef struct {
  int x;
  int y;
  struct list_head x_axis;
  struct list_head y_axis;
  int size;
  int used;
  int avail;
  struct list_head u_axis;
  struct list_head a_axis;
} node_t;

#if 0
static void print_node(node_t *n) {
  printf("%p:", n);
  printf(" %d[%p,%p,%p]", n->x, &n->x_axis, n->x_axis.next, n->x_axis.prev);
  printf(" %d[%p,%p,%p]", n->y, &n->y_axis, n->y_axis.next, n->y_axis.prev);
  printf(" %d", n->size);
  printf(" %d[%p,%p,%p]", n->used, &n->u_axis, n->u_axis.next, n->u_axis.prev);
  printf(" %d[%p,%p,%p]", n->avail, &n->a_axis, n->a_axis.next, n->a_axis.prev);
  printf("\n");
}
#endif

static int cmp_node_x_axis(struct list_head *a, struct list_head *b)
{
  node_t *an = list_entry(a, node_t, x_axis);
  node_t *bn = list_entry(b, node_t, x_axis);

//printf("X(%d,%d) vs (%d,%d)\n", an->x, an->y, bn->x, bn->y);
  if (an->y < bn->y)
    return -1;
  else if (an->y > bn->y)
    return 1;
  return 0;
}

static int cmp_node_y_axis(struct list_head *a, struct list_head *b)
{
  node_t *an = list_entry(a, node_t, y_axis);
  node_t *bn = list_entry(b, node_t, y_axis);

//printf("Y(%d,%d) vs (%d,%d)\n", an->x, an->y, bn->x, bn->y);
  if (an->x < bn->x)
    return -1;
  else if (an->x > bn->x)
    return 1;
  return 0;
}

static int cmp_node_u_axis(struct list_head *a, struct list_head *b)
{
  node_t *an = list_entry(a, node_t, u_axis);
  node_t *bn = list_entry(b, node_t, u_axis);

  if (an->used < bn->used)
    return -1;
  else if (an->used > bn->used)
    return 1;
  return 0;
}

static int cmp_node_a_axis(struct list_head *a, struct list_head *b)
{
  node_t *an = list_entry(a, node_t, a_axis);
  node_t *bn = list_entry(b, node_t, a_axis);

  if (an->avail < bn->avail)
    return -1;
  else if (an->avail > bn->avail)
    return 1;
  return 0;
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };

  struct list_head **x_axis;
  int x_max;
  struct list_head **y_axis;
  int y_max;
  struct list_head u_axis;
  struct list_head a_axis;
  node_t *tmpnode;
  int i;
  //int j;
  //struct list_head *tmplist;
  struct list_head *tmpalist;
  node_t *tmpanode;
  struct list_head *tmpulist;
  node_t *tmpunode;
  int num_viable;

  if (argc < 2)
    filename = DEFAULT_INPUT;
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  x_axis = NULL;
  x_max = -1;
  y_axis = NULL;
  y_max = -1;
  INIT_LIST_HEAD(&u_axis);
  INIT_LIST_HEAD(&a_axis);

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    if (strncmp(line, "/dev/grid/node-x", 16) != 0) {
      continue;
    }

    tmpnode = malloc(sizeof(*tmpnode));
    if (tmpnode == NULL) {
      printf("Error: Out of memory (node)\n");
      return 1;
    }

    ret = sscanf(line, "/dev/grid/node-x%d-y%d %dT %dT %dT", &tmpnode->x, &tmpnode->y,
                   &tmpnode->size, &tmpnode->used, &tmpnode->avail);
    if (ret != 5) {
      printf("Error: Expected 5, scanned %d instead\n", ret);
      return 1;
    }
//printf(":%s: %p: x%d y%d s%d u%d a%d\n", line, tmpnode, tmpnode->x, tmpnode->y, tmpnode->size, tmpnode->used, tmpnode->avail);

    if (tmpnode->x > x_max) {
      x_axis = realloc(x_axis, (tmpnode->x + 1) * sizeof(*x_axis));
//printf("> x realloc %d %lu %p\n", (tmpnode->x + 1), (tmpnode->x + 1) * sizeof(*x_axis), x_axis);
      if (x_axis == NULL) {
        printf("Error: Out of memory (x_axis)\n");
        return 1;
      }
      for (i = x_max + 1; i <= tmpnode->x; i++) {
        x_axis[i] = malloc(sizeof(*(x_axis[i])));
//printf("> malloc(%lu) = %p\n", sizeof(*(x_axis[i])), x_axis[i]);
        INIT_LIST_HEAD(x_axis[i]);
      }
      x_max = tmpnode->x;
    }
    if (tmpnode->y > y_max) {
      y_axis = realloc(y_axis, (tmpnode->y + 1) * sizeof(*y_axis));
//printf("> y realloc %d %lu %p\n", (tmpnode->y + 1), (tmpnode->y + 1) * sizeof(*y_axis), y_axis);
      if (y_axis == NULL) {
        printf("Error: Out of memory (y_axis)\n");
        return 1;
      }
      for (i = y_max + 1; i <= tmpnode->y; i++) {
        y_axis[i] = malloc(sizeof(*(y_axis[i])));
//printf("> malloc(%lu) = %p\n", sizeof(*(y_axis[i])), y_axis[i]);
        INIT_LIST_HEAD(y_axis[i]);
      }
      y_max = tmpnode->y;
    }

//printf("Add to x_axis %d\n", tmpnode->x);
    list_add_sorted(&tmpnode->x_axis, x_axis[tmpnode->x], cmp_node_x_axis);
//printf("Add to y_axis %d\n", tmpnode->y);
    list_add_sorted(&tmpnode->y_axis, y_axis[tmpnode->y], cmp_node_y_axis);
    list_add_sorted(&tmpnode->u_axis, &u_axis, cmp_node_u_axis);
    list_add_sorted(&tmpnode->a_axis, &a_axis, cmp_node_a_axis);

//print_node(tmpnode);

#if 0
    // Print grid
    for (i = y_max; i >= 0; i--) {
      printf("%2d: ", i);
      for (j = 0, tmplist = y_axis[i]->next; j <= x_max; j++) {
        if (j != 0) {
          printf(",");
        }
        //if ((tmplist == y_axis[i]) || (tmpnode->x != j))
        if (tmplist == y_axis[i]) {
          //printf("[  ,  ,   ,   ,   ]");
          printf("[  ,  ]");
        } else {
          tmpnode = list_entry(tmplist, node_t, y_axis);
          if (tmpnode->x != j) {
            //printf("[%2d,%2d,   ,   ,   ]", tmpnode->x, j);
            printf("[%2d^%2d]", tmpnode->x, j);
          } else {
            //printf("[%2d,%2d,%3d,%3d,%3d]", tmpnode->x, tmpnode->y, tmpnode->size, tmpnode->used,
                     //tmpnode->avail);
            printf("[%2d,%2d]", tmpnode->x, tmpnode->y);
            tmplist = tmplist->next;
          }
        }
      }
      printf("\n");
    }
#endif
#if 0
    list_for_each(tmplist, &u_axis) {
      tmpnode = list_entry(tmplist, node_t, u_axis);
      printf("[%2d,%2d,%3d,%3d,%3d]", tmpnode->x, tmpnode->y, tmpnode->size, tmpnode->used,
               tmpnode->avail);
    }
#endif
#if 0
    list_for_each(tmplist, &a_axis) {
      tmpnode = list_entry(tmplist, node_t, a_axis);
      printf("[%2d,%2d,%3d,%3d,%3d]", tmpnode->x, tmpnode->y, tmpnode->size, tmpnode->used,
               tmpnode->avail);
    }
#endif
    //printf("\n\n");
  }

//printf("x_max %d y_max %d\n", x_max, y_max);
  // Find viable node-pairs
  num_viable = 0;
  list_for_each(tmpulist, &u_axis) {
    tmpunode = list_entry(tmpulist, node_t, u_axis);
    if (tmpunode->used == 0) {
//printf("Zero [%2d,%2d,%3d,%3d,%3d]\n", tmpunode->x, tmpunode->y, tmpunode->size, tmpunode->used, tmpunode->avail);
      continue;
    }
    list_for_each_prev(tmpalist, &a_axis) {
      tmpanode = list_entry(tmpalist, node_t, a_axis);
      if (tmpanode->x == tmpunode->x && tmpanode->y == tmpunode->y) {
//printf("Idem [%2d,%2d,%3d,%3d,%3d] [%2d,%2d,%3d,%3d,%3d]\n", tmpunode->x, tmpunode->y, tmpunode->size, tmpunode->used, tmpunode->avail, tmpanode->x, tmpanode->y, tmpanode->size, tmpanode->used, tmpanode->avail);
        continue;
      } else if (tmpunode->used > tmpanode->avail) {
//printf("ToLg [%2d,%2d,%3d,%3d,%3d] [%2d,%2d,%3d,%3d,%3d]\n", tmpunode->x, tmpunode->y, tmpunode->size, tmpunode->used, tmpunode->avail, tmpanode->x, tmpanode->y, tmpanode->size, tmpanode->used, tmpanode->avail);
        break;
      }
//printf("Ok   [%2d,%2d,%3d,%3d,%3d] [%2d,%2d,%3d,%3d,%3d]\n", tmpunode->x, tmpunode->y, tmpunode->size, tmpunode->used, tmpunode->avail, tmpanode->x, tmpanode->y, tmpanode->size, tmpanode->used, tmpanode->avail);
      num_viable++;
    }
  }
  printf("Number of viable pairs: %d\n", num_viable);

  return 0;
}
