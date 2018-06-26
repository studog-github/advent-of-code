#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  char *data;
  int data_len;
  int disk_size;

  if (argc < 2)
    filename = "16-a-input";
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
    for (i = 0; line[i] != ' '; i++) {
      if (line[i] == '\0') {
        printf("Error: Unexpected EOF in '%s'\n", line);
        return 1;
      }
    }
    
    data_len = i;
    data = malloc((data_len + 1) * sizeof(*data));
    if (data == NULL) {
      printf("Error: Out of memory (initial data)\n");
      return 1;
    }
    strncpy(data, line, data_len);

    i++;
    ret = sscanf(&line[i], "%d", &disk_size);
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead (disk space)\n", ret);
      return 1;
    }
//printf("salt [%s] size %d\n", data, disk_size);

    while (data_len < disk_size) {
//printf(":%s:%d\n", data, data_len);
      data_len = 2 * data_len + 1;
      data = realloc(data, (data_len + 1) * sizeof(*data));
      if (data == NULL) {
        printf("Error: Out of memory (realloc)\n");
        return 1;
      }
      for (i = 0; i < data_len / 2; i++)
        data[data_len - 1 - i] = (data[i] == '0' ? '1' : '0');
      data[i] = '0';
      data[data_len] = '\0';
    }
//printf(":%s:%d\n", data, data_len);

    for (data_len = disk_size; data_len % 2 == 0; data_len /= 2) {
      for (i = 0; i < data_len; i += 2) {
        if (data[i] == data[i + 1]) {
          data[i / 2] = '1';
        } else {
          data[i / 2] = '0';
        }
      }
      data[i / 2] = '\0';
//printf(":%s:%d\n", data, data_len / 2);
    }
printf(":%s:%d\n", data, data_len);
  }

  return 0;
}
