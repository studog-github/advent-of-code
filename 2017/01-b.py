#!/usr/bin/python

PROBLEM_NUMBER = "01-b"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def digit_to_sum(list):
    pass

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
    input_str = input.read().strip()
    input_len = len(input_str)
    step = input_len / 2

    # Main loop

    sum = 0
    for i in range(input_len):
        j = (i + step) % input_len
        if (input_str[i] == input_str[j]):
            sum += int(input_str[i])

    print sum

    return 0


if __name__ == "__main__":
    sys.exit(main())
