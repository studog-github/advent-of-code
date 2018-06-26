#!/usr/bin/python

PROBLEM_NUMBER = "11-a"

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

    path = input.readline().strip().split(",")

    x = 0
    y = 0

    for step in path:
        if step == 'n':
            y += 1
        elif step == 'ne':
            x += 1
            y += 1
        elif step == 'se':
            x += 1
        elif step == 's':
            y -= 1
        elif step == 'sw':
            x -= 1
            y -= 1
        elif step == 'nw':
            x -= 1
        else:
            print "ERROR: Unrecognized step '%s'" % step
            sys.exit(1)
        #print step, "=>", x, y

    print max(abs(x), abs(y), abs(x - y))

    return 0


if __name__ == "__main__":
    sys.exit(main())
