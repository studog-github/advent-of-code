#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define LINE_SIZE  10240
#define LINE_SIZE_S  "10240"

#define CPU_NUM_REGS  4

typedef struct _cpu_t {
  int regs[CPU_NUM_REGS];
} cpu_t;

static void show_cpu(cpu_t *cpu) {
  int i;

  for (i = 0; i < CPU_NUM_REGS; i++) {
    printf("%c: %10d %08x", i + 'a', cpu->regs[i], cpu->regs[i]);
    if (i != 3)
      printf("  ");
    else
      printf("\n");
  }
}

typedef enum _op_t {
  OP_CPY,
  OP_INC,
  OP_DEC,
  OP_JNZ,
  OP_TGL,
} op_t;

typedef struct _inst_t {
  op_t op;
  bool src_is_reg;
  int src;
  bool dst_is_reg;
  int dst;
} inst_t;

#define SHOW_REGVAL(srcdst)                     \
          if (inst->srcdst ## _is_reg)          \
            printf(" %c", inst->srcdst + 'a');  \
          else                                  \
            printf(" %d", inst->srcdst);

#if 0
static void show_instruction(inst_t *inst) {
  switch (inst->op) {
    case OP_CPY:
      printf("cpy");
      SHOW_REGVAL(src);
      SHOW_REGVAL(dst);
      printf("\n");
      break;
    case OP_INC:
      printf("inc");
      SHOW_REGVAL(dst);
      printf("\n");
      break;
    case OP_DEC:
      printf("dec");
      SHOW_REGVAL(dst);
      printf("\n");
      break;
    case OP_JNZ:
      printf("jnz");
      SHOW_REGVAL(src);
      SHOW_REGVAL(dst);
      printf("\n");
      break;
    case OP_TGL:
      printf("tgl");
      SHOW_REGVAL(dst);
      printf("\n");
      break;
    default:
      printf("Error: Internal error: instruction not recognized '%d'\n", inst->op);
      break;
  }
}
#endif

#if 0
static void show_program(inst_t *program, int num_insts) {
  int i;

  for (i = 0; i < num_insts; i++) {
    printf("%08x: ", i);
    show_instruction(&program[i]);
  }
}
#endif

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
    filename = "23-a-input";
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

#define GET_REG(srcdst)                                                \
          if ((line[i] >= 'a') && (line[i] <= 'd')) {                  \
            program[num_insts].srcdst ## _is_reg = true;               \
            program[num_insts].srcdst = line[i] - 'a';                 \
            i += 2;                                                    \
          } else {                                                     \
            printf("Error: Expected register, got '%s'\n", &line[i]);  \
            return 1;                                                  \
          }

#define GET_REGVAL(srcdst)                                                      \
          if ((line[i] >= 'a') && (line[i] <= 'd')) {                           \
            program[num_insts].srcdst ## _is_reg = true;                        \
            program[num_insts].srcdst = line[i] - 'a';                          \
            i += 2;                                                             \
          } else if ((line[i] >= '0' && line[i] <= '9') || line[i] == '-' ||    \
                     line[i] == '+') {                                          \
            program[num_insts].srcdst ## _is_reg = false;                       \
            program[num_insts].srcdst = atoi(&line[i]);                         \
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
      GET_REGVAL(src);
      GET_REG(dst);
    } else if (!strncmp(line, "inc ", 4)) {
      program[num_insts].op = OP_INC;
      i = 4;
      GET_REG(dst);
    } else if (!strncmp(line, "dec ", 4)) {
      program[num_insts].op = OP_DEC;
      i = 4;
      GET_REG(dst);
    } else if (!strncmp(line, "jnz ", 4)) {
      program[num_insts].op = OP_JNZ;
      i = 4;
      GET_REGVAL(src);
      GET_REGVAL(dst);
/*
Move this to later
      if (program[num_insts].dst == 0) {
        printf("Error: jnz with 0 offset will deadlock when it triggers\n");
        return 1;
      }
*/
    } else if (!strncmp(line, "tgl ", 4)) {
      program[num_insts].op = OP_TGL;
      i = 4;
      GET_REGVAL(dst);
    } else {
      printf("Error: Unrecognized instruction '%s'\n", line);
      return 1;
    }

    num_insts++;
  }
printf("\n");

  memset(&cpu, 0, sizeof(cpu));

  for (i = 0; i < num_insts; i++) {
//show_program(program, num_insts);
//show_cpu(&cpu);
//printf("\n");
    //printf("%08x: ", i);
    //show_instruction(&program[i]);
    switch (program[i].op) {
      case OP_CPY:
        if (!program[i].dst_is_reg) {
          printf("cpy has invalid dst %d, skipping\n", program[i].dst);
          break;
        }
        if (program[i].src_is_reg)
          cpu.regs[program[i].dst] = cpu.regs[program[i].src];
        else
          cpu.regs[program[i].dst] = program[i].src;
        break;
      case OP_INC:
        if (!program[i].dst_is_reg) {
          printf("cpy has invalid dst %d, skipping\n", program[i].dst);
          break;
        }
        cpu.regs[program[i].dst]++;
        break;
      case OP_DEC:
        cpu.regs[program[i].dst]--;
        break;
      case OP_JNZ:
        if (program[i].src_is_reg)
          j = cpu.regs[program[i].src];
        else
          j = program[i].src;
        if (j != 0) {
          if (program[i].dst_is_reg)
            j = cpu.regs[program[i].dst];
          else
            j = program[i].dst;
          i += j - 1; // minus 1 to account for the loop increment
        }
        break;
      case OP_TGL:
        if (program[i].dst_is_reg)
          j = cpu.regs[program[i].dst];
        else
          j = program[i].dst;
        j += i;
        if (j < 0 || j >= num_insts) {
          printf("tgl value '%d' out of range, skipping\n", j);
          break;
        }
        switch (program[j].op) {
          case OP_CPY: // cpy x y => jnz x y
            program[j].op = OP_JNZ;
            break;
          case OP_INC: // inc x   => dec x
            program[j].op = OP_DEC;
            break;
          case OP_DEC: // dec x   => inc x
            program[j].op = OP_INC;
            break;
          case OP_JNZ: // jnz x y => cpy x y
            program[j].op = OP_CPY;
            break;
          case OP_TGL: // tgl x   => inc x
            program[j].op = OP_INC;
            break;
          default:
            printf("Error: Internal error: tgl target not recognized '%d'\n", program[i].op);
            return 1;
            break;
        }
        break;
      default:
        printf("Error: Internal error: instruction not recognized '%d'\n", program[i].op);
        return 1;
        break;
    }
    //show_cpu(&cpu);
    //printf("\n");
  }

  show_cpu(&cpu);

  return 0;
}
