#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>

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

// Missing from ncurses.h
#define COLOR_DEFAULT  -1
#define KEY_ESC  27

static inline void **malloc2d(int rows, int cols, size_t size)
{
  void **arr;
  int i;
  size_t dim_size;
  size_t header_size;
  size_t data_size;

  dim_size = 2 * sizeof(void *);
  header_size = rows * sizeof(void *);
  data_size = rows * cols * size;

  arr = malloc(dim_size + header_size + data_size);
  if (arr == NULL) {
    return arr;
  }

  // WARNING: Here be trickiness
  // The rows and cols values are stored at the start of the allocated block, but
  // the pointer returned is two ints into the block, effectively "hiding" the
  // values. This allows the size to travel with the array, but not get in the way
  // of using []s to index into the array.
  // *facepalm* I've clearly subconsciously copied Linux's list implementation. So
  // not all that tricky after all. As you were.
  *((int *)(&arr[0])) = rows;
  *((int *)(&arr[1])) = cols;
  arr += 2;

  for (i = 0; i < rows; i++) {
    arr[i] = ((unsigned char *)arr) + header_size + (i * cols * size);
  }

  return arr;
}

#define getrows2d(m) *((int *)(((void **)(m)) - 2))
#define getcols2d(m) *((int *)(((void **)(m)) - 1))

#define DEFAULT_INPUT "24-a-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

typedef struct _mtile_t {
  char disp;
  // colour to display as?
} mtile_t;

typedef struct _vertex_t {
  int id;
  int row;
  int col;
  struct list_head edges;
  struct list_head all_vertexes;
} vertex_t;

typedef struct _edge_t {
  int id;
  int weight;
  struct list_head vertexes;
  struct list_head all_edges;
} edge_t;

static bool add_vertex(vertex_t **graph, int id, int r, int c)
{
  vertex_t *v;

  v = malloc(sizeof(*v));
  if (!v)
    return false;

  v->id = id;
  v->row = r;
  v->col = c;

  if (graph = NULL) {
    *graph = v;
    return true;
  }

  list_add_sorted(v, 
static inline void list_add_sorted(struct list_head *new, struct list_head *head,
                          int (cmp)(struct list_head *a, struct list_head *b)) {
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };

  char **lines;
  int num_lines;

  struct list_head *graph;
  int num_aps;

  mtile_t **maze;
  int rows;
  int cols;

  int i;
  int j;
  int key;

  if (argc < 2)
    filename = DEFAULT_INPUT;
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  graph = NULL;
  num_aps = 0;

  lines = NULL;
  num_lines = 0;

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    lines = realloc(lines, (num_lines + 1) * sizeof(*lines));
    if (lines == NULL) {
      printf("Error: Out of memory (realloc lines)\n");
      return 1;
    }
    lines[num_lines] = strdup(line);
    if (lines[num_lines] == NULL) {
      printf("Error: Out of memory (strdup lines)\n");
      return 1;
    }
    num_lines++;
  }

  rows = num_lines;
  cols = strlen(lines[0]);
  maze = (mtile_t **)malloc2d(rows, cols, sizeof(**maze));
  if (maze == NULL) {
    printf("Error: Out of memory (malloc2d)\n");
    return 1;
  }

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      maze[i][j].disp = lines[i][j];
      if (lines[i][j] >= '0' && lines[i][j] <= '9') {
// TODO: Allocate vertexes instead, with maze coordinates stored
        if (!add_vertex(&graph, atoi(lines[i][j]), i, j)) {
          printf("Error: Out of memory (vertex)\n");
          return 1;
        }


#if 0
        graph = realloc(graph, (num_aps + 1) * sizeof(*graph));
        if (graph == NULL) {
          printf("Error: Out of memory (realloc aps)\n");
          return 1;
        }
        graph[num_aps].row = i;
        graph[num_aps].col = j;
#endif
        num_aps++;
      }
    }
    free(lines[i]);
  }
  free(lines);

#define REDRAW() ({                                                    \
          int i, j;                                                    \
          clear();                                                     \
          for (i = 0; i < rows; i++) {                                 \
            for (j = 0; j < cols; j++) {                               \
              if (maze[i][j].disp >= '0' && maze[i][j].disp <= '9') {  \
                attron(COLOR_PAIR(1));                                 \
                mvaddch(i, j, maze[i][j].disp);                        \
                attroff(COLOR_PAIR(1));                                \
              } else {                                                 \
                mvaddch(i, j, maze[i][j].disp);                        \
              }                                                        \
            }                                                          \
          }                                                            \
          mvprintw(rows, 0, "num_aps %d", num_aps);                    \
          refresh();                                                   \
        })

// TODO
// check returns values on ncurses calls
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  start_color();
  use_default_colors();
  //#define COLOR_DEFAULT  -1
  //#define COLOR_BLACK     0
  //#define COLOR_RED       1 OK for dead ends
  //#define COLOR_GREEN     2 OK for current moves
  //#define COLOR_YELLOW    3
  //#define COLOR_BLUE      4 too low contrast
  //#define COLOR_MAGENTA   5 too low contrast
  //#define COLOR_CYAN      6 OK for access points
  //#define COLOR_WHITE     7
  init_pair(1, COLOR_CYAN, COLOR_DEFAULT);

  //draw_maze(maze);
  //mvprintw(rows, 0, "num_aps %d", num_aps);
  REDRAW();

  // BFS search, animated

  while (1) {
    key = getch();
    if (key == 'q' || key == 'Q') {
      break;
    } else if (key == KEY_RESIZE) {
     //draw_maze(maze);
     REDRAW();
    }
  }

  endwin();

  return 0;
}
