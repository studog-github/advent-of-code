#!/usr/bin/python

PROBLEM_NUMBER = "06-b"

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

    banks = map(int, input.readline().split())
    num_banks = len(banks)
    positions = {}
    steps = 1
    positions[tuple(banks)] = steps

    while True:
        # Initial linear scan to find largest bank
        #print banks
        largest = 0
        for i in range(1, num_banks):
            if banks[i] > banks[largest]:
                largest = i

        i = largest
        blocks = banks[i]
        #print "largest", largest, "blocks", blocks
        banks[i] = 0
        #print banks
        while blocks > 0:
            i = (i + 1) % num_banks
            banks[i] += 1
            blocks -= 1

        steps += 1
        position = tuple(banks)
        if position in positions:
            break
        positions[position] = steps

    print steps, "-", positions[position], "=", steps - positions[position]

    return 0


if __name__ == "__main__":
    sys.exit(main())
