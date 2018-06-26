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
  OP_OUT,
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
    case OP_OUT:
      printf("out");
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

static bool run_program(inst_t *program, int num_ops, bool init, int *outval) {
  static cpu_t cpu;
  static int pc;
  int j;

//printf("\n%s(%p, %d, %d, %d)\n", __FUNCTION__, program, num_ops, init, *outval);
  if (init) {
//printf("Initing\n");
    memset(&cpu, 0, sizeof(cpu));
    pc = 0;
  }
//else {
//show_cpu(&cpu);
//printf("pc is %08x\n", pc);
//}

  for (/**/; pc < num_ops; pc++) {
//show_program(program, num_ops);
//show_cpu(&cpu);
//printf("\n");
    //printf("%08x: ", pc);
    //show_instruction(&program[pc]);
    switch (program[pc].op) {
      case OP_CPY:
        if (!program[pc].dst_is_reg) {
          printf("cpy has invalid dst %d, skipping\n", program[pc].dst);
          break;
        }
        if (program[pc].src_is_reg)
          cpu.regs[program[pc].dst] = cpu.regs[program[pc].src];
        else
          cpu.regs[program[pc].dst] = program[pc].src;
//show_cpu(&cpu);
        break;
      case OP_INC:
        if (!program[pc].dst_is_reg) {
          printf("cpy has invalid dst %d, skipping\n", program[pc].dst);
          break;
        }
        cpu.regs[program[pc].dst]++;
//show_cpu(&cpu);
        break;
      case OP_DEC:
        cpu.regs[program[pc].dst]--;
//show_cpu(&cpu);
        break;
      case OP_JNZ:
        if (program[pc].src_is_reg)
          j = cpu.regs[program[pc].src];
        else
          j = program[pc].src;
        if (j != 0) {
          if (program[pc].dst_is_reg)
            j = cpu.regs[program[pc].dst];
          else
            j = program[pc].dst;
          if (j == 0) {
            printf("Error: Deadlock detected: jnz has pffset 0\n");
            return true;
          } else {
            pc += j - 1; // minus 1 to account for the loop increment
          }
        }
        break;
      case OP_TGL:
        if (program[pc].dst_is_reg)
          j = cpu.regs[program[pc].dst];
        else
          j = program[pc].dst;
        j += pc;
        if (j < 0 || j >= num_ops) {
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
          case OP_TGL: // tgl x   => inc x
          case OP_OUT: // out x   => inc x
            program[j].op = OP_INC;
            break;
          case OP_JNZ: // jnz x y => cpy x y
            program[j].op = OP_CPY;
            break;
          default:
            printf("Error: Internal error: tgl target not recognized '%d'\n", program[pc].op);
            return true;
            break;
        }
        break;
      case OP_OUT:
//show_instruction(&program[pc]);
//show_cpu(&cpu);
        if (program[pc].dst_is_reg)
          j = cpu.regs[program[pc].dst];
        else
          j = program[pc].dst;
//printf("{%d}\n", j);
        *outval = j;
//show_cpu(&cpu);
        pc++;
        return false;
        break;
      default:
        printf("Error: Internal error: instruction not recognized '%d'\n", program[pc].op);
        return true;
        break;
    }
    //show_cpu(&cpu);
    //printf("\n");
  }

  show_cpu(&cpu);

  return true;
}

int main(int argc, char **argv)
{
  char *filename;
  FILE *fp;
  int ret;
  char line[LINE_SIZE + 1] = { 0 };
  int i;

  inst_t *program;
  int num_insts;

  int j;
  int outval;
  int num_matches;
  bool do_init;

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

  // Insert an initial copy to be able to set up register a
  program = realloc(program, (num_insts + 1) * sizeof(inst_t));
  program[num_insts].op = OP_CPY;
  program[num_insts].src_is_reg = false;
  program[num_insts].src = 0;
  program[num_insts].dst_is_reg = true;
  program[num_insts].dst = 0;
  num_insts++;

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
    } else if (!strncmp(line, "tgl ", 4)) {
      program[num_insts].op = OP_TGL;
      i = 4;
      GET_REGVAL(dst);
    } else if (!strncmp(line, "out ", 4)) {
      program[num_insts].op = OP_OUT;
      i = 4;
      GET_REG(dst);
    } else {
      printf("Error: Unrecognized instruction '%s'\n", line);
      return 1;
    }

    num_insts++;
  }
printf("\n");

  while (1) {
    j = 0;
    num_matches = 0;
    program[0].src++;
    do_init = true;
printf("Trying %d\n", program[0].src);
    while (!run_program(program, num_insts, do_init, &outval) && num_matches < 100) {
      do_init = false;
//printf(" [%d %d]\n", j, outval);
      if (outval != j) {
        break;
      }
      num_matches++;
      j = (j + 1) % 2;
    }
//printf("\n");
    if (num_matches == 100) {
      printf("Lowest positive integer is: %d\n", program[0].src);
      break;
    }
  }

  return 0;
}
