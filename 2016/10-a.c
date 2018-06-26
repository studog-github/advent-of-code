#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define LINE_SIZE  1024
#define LINE_SIZE_S  "1024"

#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)

typedef struct _node_t {
  int num;
  struct _node_t *next;
} node_t;

int queue(node_t **list, int num) {
  node_t *tmp;
  node_t *i;

  tmp = malloc(sizeof(node_t));
  if (!tmp)
    return 0;
  tmp->num = num;
  tmp->next = NULL;

  if (!*list) {
    *list = tmp;
  } else {
    for (i = *list; i->next; i = i->next)
      ;
    i->next = tmp;
  }

  return 1;
}

int dequeue(node_t **list) {
  node_t *tmp;
  int num;

  if (!*list)
    return -1;

  tmp = *list;
  if (tmp)
    *list = tmp->next;
  num = tmp->num;
  free(tmp);

  return num;
}

typedef struct _bot_t {
  int num1;
  int num2;
  int lowbot;
  int lowout;
  int highbot;
  int highout;
  struct _bot_t *next;
} bot_t;

void dump_bots(bot_t *bots, int highest) {
  int i;
  int count;

  for (i = 0, count = 0; i <= highest; i++) {
    if (bots[i].num1 == -1 && bots[i].num2 == -1 &&
        bots[i].lowbot == -1 && bots[i].lowout == -1 &&
        bots[i].highbot == -1 && bots[i].highout == -1)
      continue;
    printf("%3d: [%3d,%3d] [%c%3d,%c%3d]\n", i, bots[i].num1, bots[i].num2,
        (bots[i].lowbot != -1 ? 'b' : (bots[i].lowout != -1 ? 'o' : ' ')),
        (bots[i].lowbot != -1 ? bots[i].lowbot : bots[i].lowout),
        (bots[i].highbot != -1 ? 'b' : (bots[i].highout != -1 ? 'o' : ' ')),
        (bots[i].highbot != -1 ? bots[i].highbot : bots[i].highout));
    count++;
  }
  printf("%d bots\n", count);
}

void dump_outputs(int *outputs, int highest) {
  int i;

  printf(" ");
  for (i = 0; i <= highest; i++) {
    printf(" %3d", i);
  }
  printf("\n");

  printf("[");
  for (i = 0; i <= highest; i++) {
    printf(" %3d", outputs[i]);
  }
  printf(" ]\n");
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  bot_t *bots;
  int highest_bot;
  bot_t *botptr;
  node_t *exec_list;
  int *outputs;
  int highest_out;
  int *outptr;

  int value;
  int bot;

  if (argc < 2)
    filename = "10-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  bots = NULL;
  highest_bot = -1;
  exec_list = NULL;
  outputs = NULL;
  highest_out = -1;

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

    if (!strncmp(line, "value ", 6)) {
      i = 6;
      value = atoi(&line[i]);
      do { i++; } while (line[i] != ' ');
      i += 13;
      bot = atoi(&line[i]);
//printf(":%s: [%d to bot %d]\n", line, value, bot);
//printf("%d > %d\n", bot, highest_bot);
      if (bot > highest_bot) {
//printf("realloc(%p, (%d + 1) * %lu (%lu))\n", bots, bot, sizeof(bot_t), (bot + 1) * sizeof(bot_t));
        botptr = realloc(bots, (bot + 1) * sizeof(bot_t));
        if (!botptr) {
          printf("Error: Out of memory: realloc()\n");
          return 1;
        }
//printf("memset(%p + (%d + 1) (%p), -1, (%d - %d) * %lu (%lu))\n", botptr, highest_bot, botptr + (highest_bot + 1), bot, highest_bot, sizeof(bot_t), (bot - highest_bot) * sizeof(bot_t));
        memset(botptr + (highest_bot + 1), -1, (bot - highest_bot) * sizeof(bot_t));
//dump_bots(botptr, bot);
        bots = botptr;
        highest_bot = bot;
      }
      if (bots[bot].num1 == -1) {
        bots[bot].num1 = value;
      } else if (bots[bot].num2 == -1) {
        bots[bot].num2 = value;
        if (!queue(&exec_list, bot)) {
          printf("Error: Out of memory, queueing to exec_list\n");
          return 1;
        }
      } else {
        printf("Error: Attempted to assign 3 values to a bot\n");
        return 1;
      }
//dump_bots(bots, highest_bot);
    } else if (!strncmp(line, "bot ", 4)) {
      i = 4;
      bot = atoi(&line[i]);
//printf(":%s: [bot %d]\n", line, bot);
//printf("%d > %d\n", bot, highest_bot);
      if (bot > highest_bot) {
//printf("realloc(%p, (%d + 1) * %lu (%lu))\n", bots, bot, sizeof(bot_t), (bot + 1) * sizeof(bot_t));
        botptr = realloc(bots, (bot + 1) * sizeof(bot_t));
        if (!botptr) {
          printf("Error: Out of memory: realloc()\n");
          return 1;
        }
//printf("memset(%p + (%d + 1) (%p), -1, (%d - %d) * %lu (%lu))\n", botptr, highest_bot, botptr + (highest_bot + 1), bot, highest_bot, sizeof(bot_t), (bot - highest_bot) * sizeof(bot_t));
        memset(botptr + (highest_bot + 1), -1, (bot - highest_bot) * sizeof(bot_t));
//dump_bots(botptr, bot);
        bots = botptr;
        highest_bot = bot;
      }
      do { i++; } while (line[i] != ' ');
      i += 14;
      if (!strncmp(&line[i], "bot ", 4)) {
        i += 4;
        bots[bot].lowbot = atoi(&line[i]);
      } else if (!strncmp(&line[i], "output ", 7)) {
        i += 7;
        value = atoi(&line[i]);
        bots[bot].lowout = value;
        if (value > highest_out) {
          outptr = realloc(outputs, (value + 1) * sizeof(int));
          if (!outptr) {
            printf("Error: Out of memoryL realloc90\n");
            return 1;
          }
          memset(outptr + (highest_out + 1), -1, (value - highest_out) * sizeof(int));
          outputs = outptr;
          highest_out = value;
        }
      } else {
        printf("Error: Unrecognized output type '%s'\n", &line[i]);
        return 1;
      }
      do { i++; } while (line[i] != ' ');
      i += 13;
      if (!strncmp(&line[i], "bot ", 4)) {
        i += 4;
        bots[bot].highbot = atoi(&line[i]);
      } else if (!strncmp(&line[i], "output ", 7)) {
        i += 7;
        bots[bot].highout = atoi(&line[i]);
        if (value > highest_out) {
          outptr = realloc(outputs, (value + 1) * sizeof(int));
          if (!outptr) {
            printf("Error: Out of memoryL realloc90\n");
            return 1;
          }
          memset(outptr + (highest_out + 1), -1, (value - highest_out) * sizeof(int));
          outputs = outptr;
          highest_out = value;
        }
      } else {
        printf("Error: Unrecognized output type '%s'\n", &line[i]);
        return 1;
      }
//dump_bots(bots, highest_bot);
    } else {
      printf("Error: Unrecognized instruction '%s'\n", line);
      return 1;
    }
  }

//printf("Executing list\n");
  while (exec_list) {
    bot = dequeue(&exec_list);
    if (bot == -1) {
      printf("Error: Exec_list apparently empty\n");
      break;
    }
//printf("%3d: [%3d,%3d] [%c%3d,%c%3d]\n", bot, bots[bot].num1, bots[bot].num2,
  //(bots[bot].lowbot != -1 ? 'b' : (bots[bot].lowout != -1 ? 'o' : ' ')),
  //(bots[bot].lowbot != -1 ? bots[bot].lowbot : bots[bot].lowout),
  //(bots[bot].highbot != -1 ? 'b' : (bots[bot].highout != -1 ? 'o' : ' ')),
  //(bots[bot].highbot != -1 ? bots[bot].highbot : bots[bot].highout));
    if ((bots[bot].num1 == 17 && bots[bot].num2 == 61) ||
        (bots[bot].num1 == 61 && bots[bot].num2 == 17)) {
      printf("%3d: [%3d,%3d] [%c%3d,%c%3d]\n", bot, bots[bot].num1, bots[bot].num2,
        (bots[bot].lowbot != -1 ? 'b' : (bots[bot].lowout != -1 ? 'o' : ' ')),
        (bots[bot].lowbot != -1 ? bots[bot].lowbot : bots[bot].lowout),
        (bots[bot].highbot != -1 ? 'b' : (bots[bot].highout != -1 ? 'o' : ' ')),
        (bots[bot].highbot != -1 ? bots[bot].highbot : bots[bot].highout));
    }
    value = MIN(bots[bot].num1, bots[bot].num2);
    if (bots[bot].lowbot != -1) {
      if (bots[bots[bot].lowbot].num1 == -1) {
        bots[bots[bot].lowbot].num1 = value;
      } else if (bots[bots[bot].lowbot].num2 == -1) {
        bots[bots[bot].lowbot].num2 = value;
        if (!queue(&exec_list, bots[bot].lowbot)) {
          printf("Error: Out of memory, queueing to exec_list\n");
          return 1;
        }
      } else {
        printf("Error: Attempted to assign 3 values to a bot\n");
        return 1;
      }
    } else {
      outputs[bots[bot].lowout] = value;
    }
    value = MAX(bots[bot].num1, bots[bot].num2);
    if (bots[bot].highbot != -1) {
      if (bots[bots[bot].highbot].num1 == -1) {
        bots[bots[bot].highbot].num1 = value;
      } else if (bots[bots[bot].highbot].num2 == -1) {
        bots[bots[bot].highbot].num2 = value;
        if (!queue(&exec_list, bots[bot].highbot)) {
          printf("Error: Out of memory, queueing to exec_list\n");
          return 1;
        }
      } else {
        printf("Error: Attempted to assign 3 values to a bot\n");
        return 1;
      }
    } else {
      outputs[bots[bot].highout] = value;
    }
  }

//printf("Final bot list\n");
  dump_bots(bots, highest_bot);
//printf("Final outputs\n");
  dump_outputs(outputs, highest_out);

  printf("0 * 1 * 2 == %d\n", outputs[0] * outputs[1] * outputs[2]);

  return 0;
}
