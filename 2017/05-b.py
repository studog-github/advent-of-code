#!/usr/bin/python

PROBLEM_NUMBER = "05-b"

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

    instructions = []
    for line in input:
        instructions.append(int(line.strip()))

    #print instructions

    ip = 0
    steps = 0

    while True:
        try:
            jump = instructions[ip]
        except IndexError:
            break
        if jump >= 3:
            instructions[ip] -= 1
        else:
            instructions[ip] += 1
        ip += jump
        steps += 1

    print steps

    return 0


if __name__ == "__main__":
    sys.exit(main())
