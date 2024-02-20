#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_SIZE  128

typedef enum {
  OP_NONE,
  OP_ASSIGN,
  OP_AND,
  OP_OR,
  OP_LSHIFT,
  OP_RSHIFT,
  OP_NOT,
} op_t;

char *optoa(op_t op) {
  switch (op) {
    case OP_NONE: return "  OP_NONE";
    case OP_ASSIGN: return "OP_ASSIGN";
    case OP_AND: return "   OP_AND";
    case OP_OR: return "    OP_OR";
    case OP_LSHIFT: return "OP_LSHIFT";
    case OP_RSHIFT: return "OP_RSHIFT";
    case OP_NOT: return "OP_NOT";
    default: return " <BAD OP>";
  }
}

typedef struct _sig_t {
  char name[MAX_TOKEN_SIZE];
  unsigned short value;
  struct _sig_t *next;
  struct _sig_t *prev;
} sig_t;

void dump_list(sig_t *list) {
  sig_t *p;
  int count;

  printf("Signals:\n");
  for (p = list, count = 0; p; p = p->next, count++)
    printf("%p: %s -> %5d %04x\n", p, p->name, p->value, p->value);
  printf("%d signals\n", count);
}

sig_t *find_signal(char *name, sig_t *list) {
  sig_t *p;

  for (p = list; p; p = p->next)
    if (!strcmp(name, p->name))
      break;
  return p;
}

void insert_signal(sig_t *elem, sig_t **list) {
  sig_t *p;
  int ret;

  if (!*list) {
    *list = elem;
    elem->next = NULL;
    elem->prev = NULL;
    return;
  }

  for (p = *list; p; p = p->next) {
    ret = strcmp(elem->name, p->name);
//printf("%s: en: %s pn %s ret %d\n", __FUNCTION__, elem->name, p->name, ret);
    if (!ret) {
      // Shouldn't happen; overwrite existing value
      printf("Warning: '%s' already in list\n", elem->name);
      p->value = elem->value;
      free(elem);
      return;
    } else if (ret < 0) {
      // Insert in front of p
      elem->next = p;
      elem->prev = p->prev;
      if (p->prev)
        p->prev->next = elem;
      p->prev = elem;
      if (*list == p)
        *list = elem;
      return;
    } else if (!p->next) {
      // End of list, insert after p
      elem->next = p->next;
      elem->prev = p;
      p->next = elem;
      return;
    }
  }

  // Shouldn't happen; ignore
  printf("Error: Nowhere to insert '%s'\n", elem->name);
  free(elem);
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;

  char token[MAX_TOKEN_SIZE];
  unsigned short signal_value;
  int shift_value;
  op_t op;

  sig_t *signals;
  sig_t *sigptr;

  if (argc < 2)
    filename = "07-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  signals = NULL;
  op = OP_NONE;

  while (1) {
    ret = fscanf(fp, "%s\n", token);
    if (ret == EOF) {
      break;
    } else if (ret != 1) {
      printf("Error: Expected 1 token, scanned %d instead\n", ret);
      return 1;
    }
//printf(":%s:%5d %04x:%s:\n", optoa(op), signal_value, signal_value, token);


    if (token[0] >= '0' && token[0] <= '9') {
      if (op == OP_NONE) {
        ret = sscanf(token, "%hu", &signal_value);
        if (ret != 1) {
          printf("Error: Expected 1 <sigval>, scanned %d instead\n", ret);
          return 1;
        }
      } else if (op == OP_LSHIFT) {
        ret = sscanf(token, "%d", &shift_value);
        if (ret != 1) {
          printf("Error: Expected 1 <shiftval>, scanned %d instead\n", ret);
          return 1;
        }
        signal_value <<= shift_value;
        op = OP_NONE;
      } else if (op == OP_RSHIFT) {
        ret = sscanf(token, "%d", &shift_value);
        if (ret != 1) {
          printf("Error: Expected 1 <shiftval>, scanned %d instead\n", ret);
          return 1;
        }
        signal_value >>= shift_value;
        op = OP_NONE;
      } else {
        printf("Error: Unhandled op '%s' during <value>\n", optoa(op));
        return 1;
      }


    } else if (token[0] == '-' && token[1] == '>') {
      op = OP_ASSIGN;


    } else if (token[0] >= 'a' && token[0] <= 'z') {
      if (op == OP_NONE) {
        sigptr = find_signal(token, signals);
        if (!sigptr) {
          printf("Error: Signal '%s' doesn't exist\n", token);
          return 1;
        }
        signal_value = sigptr->value;
      } else if (op == OP_ASSIGN) {
        sigptr = find_signal(token, signals);
        if (!sigptr) {
          sigptr = malloc(sizeof(sig_t));
          if (!sigptr) {
            printf("Error: Out of memory during <signame>\n");
            return 1;
          }
          memset(sigptr, 0, sizeof(sig_t));
          strcpy(sigptr->name, token);
          insert_signal(sigptr, &signals);
        }
        sigptr->value = signal_value;
        op = OP_NONE;
//dump_list(signals);
      } else if (op == OP_AND) {
        sigptr = find_signal(token, signals);
        if (!sigptr) {
          printf("Error: Signal '%s' doesn't exist\n", token);
          return 1;
        }
        signal_value &= sigptr->value;
        op = OP_NONE;
      } else if (op == OP_OR) {
        sigptr = find_signal(token, signals);
        if (!sigptr) {
          printf("Error: Signal '%s' doesn't exist\n", token);
          return 1;
        }
        signal_value |= sigptr->value;
        op = OP_NONE;
      } else if (op == OP_NOT) {
        sigptr = find_signal(token, signals);
        if (!sigptr) {
          printf("Error: Signal '%s' doesn't exist\n", token);
          return 1;
        }
        signal_value = ~sigptr->value;
        op = OP_NONE;
      } else {
        printf("Error: Unhandled op '%s' during <token>\n", optoa(op));
        return 1;
      }


    } else if (token[0] >= 'A' && token[0] <= 'Z') {
      if (!strncmp(token, "AND", 3)) {
        op = OP_AND;
      } else if (!strncmp(token, "OR", 2)) {
        op = OP_OR;
      } else if (!strncmp(token, "LSHIFT", 6)) {
        op = OP_LSHIFT;
      } else if (!strncmp(token, "RSHIFT", 6)) {
        op = OP_RSHIFT;
      } else if (!strncmp(token, "NOT", 3)) {
        op = OP_NOT;
      } else {
        printf("Error: Unrecognized op '%s' during <token>\n", token);
        return 1;
      }


    } else {
      printf("Error: Unrecognized token '%s'\n", token);
      return 1;
    }
  }

  dump_list(signals);

  return 0;
}
