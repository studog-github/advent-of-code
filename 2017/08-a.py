#!/usr/bin/python

PROBLEM_NUMBER = "08-a"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def main():
    arg_parser = argparse.ArgumentParser(prog=THIS)
    arg_parser.add_argument("-f", "--file", default=DEFAULT_INPUT,
                            help="specify the input file (default: %(default)s)", metavar="file")
    args = arg_parser.parse_args()

    # Open input file

    input_path = os.path.abspath(args.file)
    if not os.path.exists(input_path):
        print("%s: Error: '%s' not found" % (THIS, args.file))
        return 1

    input = open(input_path, "r")
    if input is None:
        print("%s: Error: Couldn't open '%s'" % (THIS, input_path))
        return 1

    # Main loop

    registers = {}
    instructions = []
    for line in input:
        instruction = line.strip().split()
        instruction[2] = int(instruction[2])
        instruction[6] = int(instruction[6])
        del instruction[3]
        #print instruction
        instructions.append(instruction)
        if instruction[0] not in registers:
            registers[instruction[0]] = 0
        if instruction[3] not in registers:
            registers[instruction[3]] = 0

    #print instructions
    #print registers

    for instruction in instructions:
        #print instruction
        # Check condition
        cond_reg_val = registers[instruction[3]]
        cond_op = instruction[4]
        cond_op_val = instruction[5]
        cond_result = False
        if cond_op == '<':
            if cond_reg_val < cond_op_val:
                cond_result = True
        elif cond_op == '<=':
            if cond_reg_val <= cond_op_val:
                cond_result = True
        elif cond_op == '>':
            if cond_reg_val > cond_op_val:
                cond_result = True
        elif cond_op == '>=':
            if cond_reg_val >= cond_op_val:
                cond_result = True
        elif cond_op == '==':
            if cond_reg_val == cond_op_val:
                cond_result = True
        elif cond_op == '!=':
            if cond_reg_val != cond_op_val:
                cond_result = True
        else:
            print "Error: Unknown cond_op '%s'" % (instruction[4])
            return 1

        if cond_result:
            op_reg = instruction[0]
            op = instruction[1]
            op_val = instruction[2]
            if op == 'inc':
                registers[op_reg] += op_val
            elif op == 'dec':
                registers[op_reg] -= op_val
            #print registers

    largest = None
    for reg in registers:
        if largest is None:
            largest = reg
            continue
        if registers[reg] > registers[largest]:
            largest = reg

    print largest, registers[largest]

    return 0


if __name__ == "__main__":
    sys.exit(main())
