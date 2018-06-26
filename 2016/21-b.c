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

#define DEFAULT_INPUT "21-b-input"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  char *password;
  int password_len;
  char *tmppass;

  int x;
  int y;
  int j;

  if (argc < 2)
    filename = DEFAULT_INPUT;
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  // First line is the starting string
  ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
  if (ret == EOF)
    return 0;
  if (ret != 1) {
    printf("Error: Expected 1, scanned %d instead\n", ret);
    return 1;
  }

  password_len = strlen(line);
  password = malloc((password_len + 1) * sizeof(char));
  if (password == NULL) {
    printf("Error: Out of memory (password)\n");
    return 1;
  }
  strcpy(password, line);

  tmppass = malloc((password_len + 1) * sizeof(char));
  if (tmppass == NULL) {
    printf("Error: Out of memory (tmppass)\n");
    return 1;
  }

#define GET_INDEX(x)  ({                                                             \
          x = atoi(&line[i]);                                                        \
          if (x < 0 || x > password_len - 1) {                                       \
            printf("Error: Index '%d' out of range 0 - %d\n", x, password_len - 1);  \
            return 1;                                                                \
          }                                                                          \
        })

#define SORT_INDEX(x, y)  ({  \
          int z;              \
          if (x > y) {        \
            z = x;            \
            x = y;            \
            y = z;            \
            printf("Warning: Fixed reversed indexes\n");  \
          }                   \
        })

#define FIND_INDEX_OF(x, c)  ({                                \
          for (x = 0; x < password_len; x++) {                 \
            if (password[x] == c)                              \
              break;                                           \
          }                                                    \
          if (x == password_len) {                             \
            printf("Error: '%c' not found in password\n", c);  \
            return 1;                                          \
          }                                                    \
        })

printf(":%s:\n", password);
  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

// The following operands are inverted

printf(":%s:\n", line);
    if (strncmp(line, "swap position ", 14) == 0) {
      i = 14;
      GET_INDEX(x);
      SKIP_OVER(' ');
      i += 14;
      GET_INDEX(y);
      tmppass[0] = password[x];
      password[x] = password[y];
      password[y] = tmppass[0];


    } else if (strncmp(line, "swap letter ", 12) == 0) {
      i = 12;
      FIND_INDEX_OF(x, line[i]);
      SKIP_OVER(' ');
      i += 12;
      FIND_INDEX_OF(y, line[i]);
      tmppass[0] = password[x];
      password[x] = password[y];
      password[y] = tmppass[0];


    } else if (strncmp(line, "reverse positions ", 18) == 0) {
      i = 18;
      GET_INDEX(x);
      SKIP_OVER(' ');
      i += 8;
      GET_INDEX(y);
      SORT_INDEX(x, y);
      memcpy(&tmppass[x], &password[x], y - x + 1);
      for (j = 0; j < y - x + 1; j++)
        password[x + j] = tmppass[y - j];


    } else if (strncmp(line, "rotate left ", 12) == 0) {
      i = 12;
      GET_INDEX(x);
      x %= password_len;
      x = password_len - x;
      if (x > 0) {
        memcpy(tmppass, password, password_len);
        for (j = password_len - 1; j >= 0; j--)
          password[j] = tmppass[(j - x + password_len) % password_len];
      }


    } else if (strncmp(line, "rotate right ", 13) == 0) {
      i = 13;
      GET_INDEX(x);
      x %= password_len;
      x = password_len - x;
      if (x > 0) {
        memcpy(tmppass, password, password_len);
        for (j = 0; j < password_len; j++)
          password[j] = tmppass[(j + x) % password_len];
      }


    } else if (strncmp(line, "move position ", 14) == 0) {
      i = 14;
      GET_INDEX(x);
      SKIP_OVER(' ');
      i += 12;
      GET_INDEX(y);
      j = x; x = y; y = j;
      if (x < y) {
        tmppass[0] = password[x];
        for (j = x; j < y; j++)
          password[j] = password[j + 1];
        password[j] = tmppass[0];
      } else if (x > y) {
        tmppass[0] = password[x];
        for (j = x; j > y; j--)
          password[j] = password[j - 1];
        password[j] = tmppass[0];
      } else {
        printf("Error: Indexes '%d' and '%d' not allowed to be equal\n", x, y);
        return 1;
      }


    } else if (strncmp(line, "rotate based on position of letter ", 35) == 0) {
      i = 35;
      FIND_INDEX_OF(x, line[i]);
      switch (x) {
        default:
        case 0:
        case 1: y = 7; break;
        case 2: y = 2; break;
        case 3: y = 6; break;
        case 4: y = 1; break;
        case 5: y = 5; break;
        case 6: y = 0; break;
        case 7: y = 4; break;
      }
      memcpy(tmppass, password, password_len);
      for (j = password_len - 1; j >= 0; j--)
        password[j] = tmppass[(j - y + password_len) % password_len];


    } else {
      printf("Error: Unrecognized operation '%s'\n", line);
      return 1;
    }
printf(":%s:\n", password);
  }

  printf("Password is %s\n", password);

  return 0;
}
