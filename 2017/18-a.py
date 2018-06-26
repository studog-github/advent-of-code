#!/usr/bin/python

PROBLEM_NUMBER = "18-a"

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

    machine = {}

    insts = []
    for line in input:
        line = line.strip().split()
        if line[0] not in ['snd', 'set', 'add', 'mul', 'mod', 'rcv', 'jgz']:
            print "Error: Unrecognised instruction '%s'" % line[0]
            sys.exit(1)
        try:
            line[1] = int(line[1])
        except ValueError:
            pass
        if isinstance(line[1], int):
            machine[line[1]] = line[1]
        else:
            machine[line[1]] = 0
        if len(line) == 3:
            try:
                line[2] = int(line[2])
            except ValueError:
                pass
            if isinstance(line[2], int):
                machine[line[2]] = line[2]
            else:
                machine[line[2]] = 0
        if line[0] in ['set', 'add', 'mul', 'mod'] and isinstance(line[1], int):
            print "Error: '%s' expected register, got '%s'" % (line[0], line[1])
            sys.ext(1)
        #print line
        insts.append(tuple(line))
    #print insts
    num_insts = len(insts)
    print "num insts:", num_insts
    print machine
    print

    ip = 0
    while ip >= 0 and ip < num_insts:
        inst = insts[ip]
        print "%d: %s" % (ip, inst)
        if inst[0] == 'snd':
            machine['sound'] = machine[inst[1]]
        elif inst[0] == 'set':
            machine[inst[1]] = machine[inst[2]]
        elif inst[0] == 'add':
            machine[inst[1]] += machine[inst[2]]
        elif inst[0] == 'mul':
            machine[inst[1]] *= machine[inst[2]]
        elif inst[0] == 'mod':
            machine[inst[1]] = machine[inst[1]] % machine[inst[2]]
        elif inst[0] == 'rcv':
            if machine[inst[1]] != 0:
                machine['recover'] = machine['sound']
                break
        elif inst[0] == 'jgz':
            if machine[inst[1]] > 0:
                ip += machine[inst[2]]
                continue
        print machine
        print
        ip += 1

    print machine['recover']

    return 0


if __name__ == "__main__":
    sys.exit(main())
