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

#define DEFAULT_INPUT "21-a2-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

typedef enum _op_t {
  OP_SWAP_POS,
  OP_SWAP_LET,
  OP_ROT_LEFT,
  OP_ROT_RIGHT,
  OP_ROT_LETT,
  OP_REV_POS,
  OP_MOV_POS,
} op_t;

typedef struct _inst_t {
  op_t op;
  int idx1;
  int idx2;
  char let1;
  char let2;
} inst_t;

#if 0
static void show_instruction(inst_t *inst)
{
  switch (inst->op) {
    case OP_SWAP_POS:
      printf("swap pos %d %d\n", inst->idx1, inst->idx2);
      break;
    case OP_SWAP_LET:
      printf("swap let %c %c\n", inst->let1, inst->let2);
      break;
    case OP_ROT_LEFT:
      printf("rot left %d\n", inst->idx1);
      break;
    case OP_ROT_RIGHT:
      printf("rot rght %d\n", inst->idx1);
      break;
    case OP_ROT_LETT:
      printf("rot lett %c\n", inst->let1);
      break;
    case OP_REV_POS:
      printf("revs pos %d %d\n", inst->idx1, inst->idx2);
      break;
    case OP_MOV_POS:
      printf("move pos %d %d\n", inst->idx1, inst->idx2);
      break;
    default:
      printf("Error: Internal error: instruction not recognized '%d'\n", inst->op);
      break;
  }
}
#endif

#if 0
static void show_program(inst_t *program, int num_ops)
{
  int i;

  for (i = 0; i < num_ops; i++) {
    printf("%2d: ", i);
    show_instruction(&program[i]);
  }
}
#endif

static inline void swap(char *a, char *b)
{
  char z = *a;
  *a = *b;
  *b = z;
}

static void permute(char *s, int i, int len, char ***list, int *list_len)
{
  int j;

  if (i == len) {
    //printf("%s\n", s);
    *list = realloc(*list, (*list_len + 1) * sizeof(**list));
    (*list)[*list_len] = malloc((strlen(s) + 1) * sizeof(*s));
    strcpy((*list)[*list_len], s);
    (*list_len)++;
  } else {
    for (j = i; j <= len; j++) {
      swap(s + i, s + j);
      permute(s, i + 1, len, list, list_len);
      swap(s + i, s + j);
    }
  }
}

#define FIND_INDEX_OF(x, c)  ({                                \
          for (x = 0; x < pw_len; x++) {                       \
            if (password[x] == c)                              \
              break;                                           \
          }                                                    \
          if (x == pw_len) {                                   \
            printf("Error: '%c' not found in password\n", c);  \
            return;                                            \
          }                                                    \
        })

#define REVERSE_SUBSTR(x,y)  ({                \
          int j;                               \
          memcpy(pw_tmp, password, pw_len);    \
          for (j = 0; j < y - x + 1; j++)      \
            password[x + j] = pw_tmp[y - j];   \
        })

#define ROTATE_LEFT(x)  ({                             \
          int j;                                       \
          if (x > 0) {                                 \
            memcpy(pw_tmp, password, pw_len);          \
            for (j = 0; j < pw_len; j++)               \
              password[j] = pw_tmp[(j + x) % pw_len];  \
          }                                            \
        })

static char *pw_tmp = NULL;

static inline void rotate_left(char *pw, int len, int rotval)
{
  int j;

  if (rotval > 0) {
    memcpy(pw_tmp, pw, len);
    for (j = 0; j < len; j++)
      pw[j] = pw_tmp[(j + rotval) % len];
  }
}

static void scramble(inst_t *program, int num_ops, char *password, int pw_len)
{
  int i;

  if (pw_tmp == NULL) {
    pw_tmp = strdup(password);
    if (pw_tmp == NULL) {
      printf("Error: Out of memory (strdup)\n");
      return;
    }
  }

  for (i = 0; i < num_ops; i++) {
//printf(":%s: ", password);
//show_instruction(&program[i]);
    switch (program[i].op) {
      case OP_SWAP_LET:
        FIND_INDEX_OF(program[i].idx1, program[i].let1);
        FIND_INDEX_OF(program[i].idx2, program[i].let2);
        /* fall through */
      case OP_SWAP_POS:
        swap(&password[program[i].idx1], &password[program[i].idx2]);
        break;
      case OP_ROT_LEFT:
        rotate_left(password, pw_len, program[i].idx1);
        break;
      case OP_ROT_LETT:
        FIND_INDEX_OF(program[i].idx1, program[i].let1);
        if (program[i].idx1 >= 4)
          program[i].idx1++;
        program[i].idx1++;
        program[i].idx1 %= pw_len;
        /* fall through */
      case OP_ROT_RIGHT:
        rotate_left(password, pw_len, pw_len - program[i].idx1);
        break;
      case OP_REV_POS:
        REVERSE_SUBSTR(program[i].idx1, program[i].idx2);
        break;
      case OP_MOV_POS:
        if (program[i].idx1 < program[i].idx2) {
          rotate_left(&password[program[i].idx1], program[i].idx2 - program[i].idx1 + 1, 1);
        } else {
          rotate_left(&password[program[i].idx2], program[i].idx1 - program[i].idx2 + 1,
                      program[i].idx1 - program[i].idx2 + 1 - 1);
        }
        break;
      default:
        printf("Error: Internal error: instruction not recognized '%d'\n", program[i].op);
        break;
    }
  }
//printf(":%s:\n", password);
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  char *password;
  int password_len;

  char **password_list;
  int num_in_list;

  inst_t *program;
  int num_ops;

  if (argc < 2)
    filename = DEFAULT_INPUT;
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  program = NULL;
  num_ops = 0;

  // First line is length of password
  ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
  if (ret != 1) {
    printf("Error: Expected 1, scanned %d instead\n", ret);
    return 1;
  }
  password_len = atoi(line);

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

#define GET_INDEX(x)  ({                                                             \
          x = atoi(&line[i]);                                                        \
          if (x < 0 || x > password_len - 1) {                                       \
            printf("Error: Index '%d' out of range 0 - %d\n", x, password_len - 1);  \
            return 1;                                                                \
          }                                                                          \
        })

#define SORT_INDEX(x, y)  ({                              \
          int z;                                          \
          if (x > y) {                                    \
            z = x;                                        \
            x = y;                                        \
            y = z;                                        \
            printf("Warning: Fixed reversed indexes\n");  \
          }                                               \
        })

//printf(":%s:\n", line);
    program = realloc(program, (num_ops + 1) * sizeof(inst_t));
    if (strncmp(line, "swap position ", 14) == 0) {
      program[num_ops].op = OP_SWAP_POS;
      i = 14;
      GET_INDEX(program[num_ops].idx1);
      SKIP_OVER(' ');
      i += 14;
      GET_INDEX(program[num_ops].idx2);
    } else if (strncmp(line, "swap letter ", 12) == 0) {
      program[num_ops].op = OP_SWAP_LET;
      i = 12;
      program[num_ops].let1 = line[i];
      SKIP_OVER(' ');
      i += 12;
      program[num_ops].let2 = line[i];
    } else if (strncmp(line, "rotate left ", 12) == 0) {
      program[num_ops].op = OP_ROT_LEFT;
      i = 12;
      GET_INDEX(program[num_ops].idx1);
    } else if (strncmp(line, "rotate right ", 13) == 0) {
      program[num_ops].op = OP_ROT_RIGHT;
      i = 13;
      GET_INDEX(program[num_ops].idx1);
    } else if (strncmp(line, "rotate based on position of letter ", 35) == 0) {
      program[num_ops].op = OP_ROT_LETT;
      i = 35;
      program[num_ops].let1 = line[i];
    } else if (strncmp(line, "reverse positions ", 18) == 0) {
      program[num_ops].op = OP_REV_POS;
      i = 18;
      GET_INDEX(program[num_ops].idx1);
      SKIP_OVER(' ');
      i += 8;
      GET_INDEX(program[num_ops].idx2);
      SORT_INDEX(program[num_ops].idx1, program[num_ops].idx2);
    } else if (strncmp(line, "move position ", 14) == 0) {
      program[num_ops].op = OP_MOV_POS;
      i = 14;
      GET_INDEX(program[num_ops].idx1);
      SKIP_OVER(' ');
      i += 12;
      GET_INDEX(program[num_ops].idx2);
    } else {
      printf("Error: Unrecognized operation '%s'\n", line);
      return 1;
    }

    num_ops++;
  }

//printf("Password length: %d\n", password_len);
//show_program(program, num_ops);
  password = malloc((password_len + 1) * sizeof(*password));
  for (i = 0; i < password_len; i++)
    password[i] = i + 'a';
  password[i] = '\0';

  password_list = NULL;
  num_in_list = 0;

  permute(password, 0, password_len - 1, &password_list, &num_in_list);
//printf("Number in list: %d\n", num_in_list);
//for (i = 0; i < num_in_list; i++) {
  //printf("%d: %s\n", i, password_list[i]);
//}

  for (i = 0; i < num_in_list; i++) {
    printf("%s => ", password_list[i]);
    scramble(program, num_ops, password_list[i], password_len);
    printf("%s\n", password_list[i]);
  }

  return 0;
}
