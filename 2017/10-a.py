#!/usr/bin/python

PROBLEM_NUMBER = "10-a"

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

    list_len = int(input.readline().strip())
    l = range(list_len)
    print l

    lengths = map(int, input.readline().strip().split(','))
    #print lengths

    skip_size = 0
    start = 0

    for length in lengths:
        end = start + length
        print "start:", start, "end:", end, "length:", length, "skip_size:", skip_size
        if length <= 1:
            pass
        elif end < list_len:
            l[start:end] = reversed(l[start:end])
        else:
            l *= 2
            #print l
            l[start:end] = reversed(l[start:end])
            #print l
            l[:end - list_len] = l[list_len:end]
            #print l
            del l[list_len:]
        start = (start + length + skip_size) % list_len
        skip_size += 1
        print l
        if len(l) != list_len:
            print "ERROR: list is too short, len =", len(l)
        for i in range(list_len):
            found = False
            for j in range(list_len):
                if l[j] == i:
                    found = True
                    break
            if not found:
                print "ERROR: list is missing element", i
                break

    print l[0], "*", l[1], "=", l[0] * l[1]

    return 0


if __name__ == "__main__":
    sys.exit(main())
