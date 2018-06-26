#!/usr/bin/python

PROBLEM_NUMBER = "02-b"

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

    checksum = 0
    for line in input:
        line = sorted(map(int, line.split()))
        line_len = len(line)
        for i in range(line_len - 1):
            j = i + 1
            while (j < line_len):
                if (line[j] % line[i] == 0):
                    checksum += (line[j] / line[i])
                    break
                j += 1
            if (j != line_len):
                break
    print checksum

    return 0


if __name__ == "__main__":
    sys.exit(main())
