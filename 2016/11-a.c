#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "list.h"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

// New strategy:
// - move generation done with lists of objs
// - have is_floor_valid() to check list state
// - remove each item & pair { remove_objs() }, check floor
// - add each item & pair { insert_objs() } to other floor, check floor
// - floor valid if:
// - nothing
// - only Gs
// - only Ms
// - mixed, all Ms must be paired
//   - have is_paired()
// - elevator can be treated as a floor

#define NUM_FLOORS  4

typedef struct _obj_t {
  char type;
  char gorc;
  struct list_head all_objs;
  struct list_head loc;
} obj_t;

static int cmp_obj_t(obj_t *a, obj_t *b)
{
  if (a->type < b->type)
    return -1;
  else if (a->type > b->type)
    return 1;

  if (a->gorc < b->gorc)
    return -1;
  else if (a->gorc > b->gorc)
    return 1;

  return 0;
}

DECL_LIST_CMP(obj_t, all_objs);
DECL_LIST_CMP(obj_t, loc);

typedef struct _building_t {
  struct list_head all_objs;
  struct list_head floors[NUM_FLOORS];
  struct list_head elevator;
  int elevator_floor;
} building_t;

static void display_building(building_t *b)
{
  int i;
  struct list_head *ap;
  obj_t *ao;
  struct list_head *fp;
  obj_t *fo;

  for (i = NUM_FLOORS - 1; i >= 0; i--) {
    printf("F%d", i + 1);
    printf(" %c", (i == b->elevator_floor ? 'E' : '.'));
    fp = b->floors[i].next;
    fo = NULL;
    list_for_each(ap, &b->all_objs) {
      if (fp == &b->floors[i]) {
        printf(" . ");
        continue;
      } else if (fo == NULL) {
        fo = list_entry(fp, obj_t, loc);
      }
      ao = list_entry(ap, obj_t, all_objs);
      if (ao == fo) {
        printf(" %c%c", ao->type, ao->gorc);
        fp = fp->next;
        fo = NULL;
      } else {
        printf(" . ");
      }
    }
    printf("\n");
  }
}

static bool is_valid_floor(struct list_head *floor)
{
  struct list_head *f;
  bool have_unpaired_microchip;
  bool have_generator;
  obj_t *o;
  obj_t *p;

  have_unpaired_microchip = false;
  have_generator = false;
  p = NULL;
  list_for_each(f, floor) {
    o = list_entry(f, obj_t, loc);
    if (o->gorc == 'G') {
      if (have_unpaired_microchip)
        return false;
      have_generator = true;
    } else if ((o->gorc == 'M') && (p == NULL || p->type != o->type)) {
      if (have_generator)
        return false;
      have_unpaired_microchip = true;
    }
    p = o;
  }

  return true;
}

#if 0
static void display_list_all_objs(struct list_head *list)
{
  obj_t *o;
  bool first;

  first = true;
  list_for_each_entry(o, list, all_objs) {
    if (first) {
      printf("%p:%c%c", o, o->type, o->gorc);
      first = false;
    } else {
      printf(" -> %p:%c%c", o, o->type, o->gorc);
    }
  }
  printf("\n");
}
#endif

typedef struct _move_t {
  obj_t *obj1;
  obj_t *obj2;
  int dest;
  struct list_head list;
} move_t;

static bool solve(building_t *b, struct list_head *m)
{
  struct list_head possible_moves;
  //int i;
  //struct list_head *p;

(void)b;
(void)m;
  INIT_LIST_HEAD(&possible_moves);

  return true;
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  int floor;
  char type;
  obj_t *o;

  building_t building;
  struct list_head moves;
  int num_moves;

  if (argc < 2)
    filename = "10-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  INIT_LIST_HEAD(&building.all_objs);
  for (i = 0; i < NUM_FLOORS; i++)
    INIT_LIST_HEAD(&building.floors[i]);
  INIT_LIST_HEAD(&building.elevator);
  building.elevator_floor = 0;

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    i = 4;
    if (!strncmp(&line[i], "first", 5)) {
      floor = 0;
      i += 5;
    } else if (!strncmp(&line[i], "second", 6)) {
      floor = 1;
      i += 6;
    } else if (!strncmp(&line[i], "third", 5)) {
      floor = 2;
      i += 5;
    } else if (!strncmp(&line[i], "fourth", 6)) {
      floor = 3;
      i += 6;
    } else {
      printf("Error: Unknown floor '%s'\n", &line[i]);
      return 1;
    }

    if (!strcmp(&line[i], " floor contains nothing relevant.")) {
      continue;
    } else {
      i += 18;
    }

    while (1) {
      type = toupper(line[i]);
      do { i++; } while (line[i] != ' '); i++;
      if (!strncmp(&line[i], "generator", 9)) {
        i += 9;
        o = malloc(sizeof(*o));
        if (!o) {
          printf("Error: Out of memory, new object\n");
          return 1;
        }
        o->type = type;
        o->gorc = 'G';
        list_add_sorted(&o->all_objs, &building.all_objs, LIST_CMP(obj_t, all_objs));
        list_add_sorted(&o->loc, &building.floors[floor], LIST_CMP(obj_t, loc));
      } else if (!strncmp(&line[i], "microchip", 9)) {
        i += 9;
        o = malloc(sizeof(*o));
        if (!o) {
          printf("Error: Out of memory, new object\n");
          return 1;
        }
        o->type = type;
        o->gorc = 'M';
        list_add_sorted(&o->all_objs, &building.all_objs, LIST_CMP(obj_t, all_objs));
        list_add_sorted(&o->loc, &building.floors[floor], LIST_CMP(obj_t, loc));
      } else {
        printf("Error: Unknown goc '%s'\n", &line[i]);
        return 1;
      }
      do { i++; } while (line[i] != ' ' && line[i] != '\0');
      if (line[i] == '\0')
        break;
      i++;
      if (strncmp(&line[i], "and ", 4) == 0)
        i += 4;
      do { i++; } while (line[i] != ' ' && line[i] != '\0');
      if (line[i] == '\0')
        break;
      i++;
    }
  }

  for (i = 0; i < NUM_FLOORS; i++) {
    if (!is_valid_floor(&building.floors[i])) {
      printf("Error: Malformed input; floor %d is invalid\n", i);
      display_building(&building);
      return 1;
    }
  }

display_building(&building);

  INIT_LIST_HEAD(&moves);
  if (solve(&building, &moves)) {
    num_moves = 1;
    printf("Solution is %d moves\n", num_moves);
  } else {
    printf("No solution found\n");
  }

  return 0;
}
