#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_SIZE  10240
#define LINE_SIZE_S  "10240"

typedef enum _op_t {
  OP_CPY,
  OP_INC,
  OP_DEC,
  OP_JNZ,
} op_t;

typedef struct _inst_t {
  op_t op;
  int regref;
  int source;
  int dest;
} inst_t;

typedef struct _cpu_t {
  int regs[4];
} cpu_t;

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  inst_t *program;
  int num_insts;

  cpu_t cpu;
  int j;

  if (argc < 2)
    filename = "12-a-input";
  else
    filename = argv[1];

  fp = fopen(filename, "r");
  if (!fp) {
    printf("Error: Couldn't open '%s': %s (%d)\n", filename, strerror(errno), errno);
    return 1;
  }

  program = NULL;
  num_insts = 0;

  while (1) {
    ret = fscanf(fp, "%" LINE_SIZE_S "[^\n]\n", line);
    if (ret == EOF)
      break;
    if (ret != 1) {
      printf("Error: Expected 1, scanned %d instead\n", ret);
      return 1;
    }

#define GET_DEST_REG                                                   \
          if ((line[i] >= 'a') && (line[i] <= 'd')) {                  \
            program[num_insts].dest = line[i] - 'a';                   \
          } else {                                                     \
            printf("Error: Expected register, got '%s'\n", &line[i]);  \
            return 1;                                                  \
          }

#define GET_SOURCE_REGVAL                                                       \
          if ((line[i] >= 'a') && (line[i] <= 'd')) {                           \
            program[num_insts].regref = 1;                                      \
            program[num_insts].source = line[i] - 'a';                          \
            i += 2;                                                             \
          } else if ((line[i] >= '0') && (line[i] <= '9')) {                    \
            program[num_insts].regref = 0;                                      \
            program[num_insts].source = atoi(&line[i]);                         \
            do { i++; } while (line[i] != ' '); i++;                            \
          } else {                                                              \
            printf("Error: Expected register or value, got '%s'\n", &line[i]);  \
            return 1;                                                           \
          }

printf(":%s:%d:%lu\n", line, num_insts, (num_insts + 1) * sizeof(inst_t));
    program = realloc(program, (num_insts + 1) * sizeof(inst_t));
    if (!strncmp(line, "cpy ", 4)) {
      program[num_insts].op = OP_CPY;
      i = 4;
      GET_SOURCE_REGVAL;
      GET_DEST_REG;
    } else if (!strncmp(line, "inc ", 4)) {
      program[num_insts].op = OP_INC;
      i = 4;
      GET_DEST_REG;
    } else if (!strncmp(line, "dec ", 4)) {
      program[num_insts].op = OP_DEC;
      i = 4;
      GET_DEST_REG;
    } else if (!strncmp(line, "jnz ", 4)) {
      program[num_insts].op = OP_JNZ;
      i = 4;
      GET_SOURCE_REGVAL;
      program[num_insts].dest = atoi(&line[i]);
      if (program[num_insts].dest == 0) {
        printf("Error: jnz with 0 offset will deadlock when it triggers\n");
        return 1;
      }
    } else {
      printf("Error: Unrecognized instruction '%s'\n", line);
      return 1;
    }

    num_insts++;
  }

#if 0
  for (i = 0; i < num_insts; i++) {
    switch (program[i].op) {
      case OP_CPY:
        printf("cpy");
        if (program[i].regref)
          printf(" %c", program[i].source + 'a');
        else
          printf(" %d", program[i].source);
        printf(" %c\n", program[i].dest + 'a');
        break;
      case OP_INC:
        printf("inc %c\n", program[i].dest + 'a');
        break;
      case OP_DEC:
        printf("dec %c\n", program[i].dest + 'a');
        break;
      case OP_JNZ:
        printf("jnz");
        if (program[i].regref)
          printf(" %c", program[i].source + 'a');
        else
          printf(" %d", program[i].source);
        printf(" %d\n", program[i].dest);
        break;
      default:
        printf("Error: Internal error: instruction not recognized '%d'\n", program[i].op);
        return 1;
        break;
    }
  }
#endif

#define SHOW_REGS                                                        \
          for (j = 0; j < 4; j++) {                                      \
            printf("%c: %10d %08x", j + 'a', cpu.regs[j], cpu.regs[j]);  \
            if (j != 3)                                                  \
              printf("  ");                                              \
            else                                                         \
              printf("\n");                                              \
          }

  memset(&cpu, 0, sizeof(cpu));

  for (i = 0; i < num_insts; i++) {
#if 0
    printf("%08x: ", i);
    switch (program[i].op) {
      case OP_CPY:
        printf("cpy");
        if (program[i].regref)
          printf(" %c", program[i].source + 'a');
        else
          printf(" %d", program[i].source);
        printf(" %c\n", program[i].dest + 'a');
        break;
      case OP_INC:
        printf("inc %c\n", program[i].dest + 'a');
        break;
      case OP_DEC:
        printf("dec %c\n", program[i].dest + 'a');
        break;
      case OP_JNZ:
        printf("jnz");
        if (program[i].regref)
          printf(" %c", program[i].source + 'a');
        else
          printf(" %d", program[i].source);
        printf(" %d\n", program[i].dest);
        break;
      default:
        printf("Error: Internal error: instruction not recognized '%d'\n", program[i].op);
        return 1;
        break;
    }
#endif
    switch (program[i].op) {
      case OP_CPY:
        if (program[i].regref)
          cpu.regs[program[i].dest] = cpu.regs[program[i].source];
        else
          cpu.regs[program[i].dest] = program[i].source;
        break;
      case OP_INC:
        cpu.regs[program[i].dest]++;
        break;
      case OP_DEC:
        cpu.regs[program[i].dest]--;
        break;
      case OP_JNZ:
        if (program[i].regref)
          j = cpu.regs[program[i].source];
        else
          j = program[i].source;
        if (j != 0)
          i += program[i].dest - 1; // minus 1 to account for the loop increment
        break;
      default:
        printf("Error: Internal error: instruction not recognized '%d'\n", program[i].op);
        return 1;
        break;
    }
//SHOW_REGS;
  }

  SHOW_REGS;

  return 0;
}
