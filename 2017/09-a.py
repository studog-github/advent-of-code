#!/usr/bin/python

PROBLEM_NUMBER = "09-a"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def remove_garbage(garbage):
    if garbage[0] != '<':
        print "Error: Bad garbage at '%s'" % (garbage[0])
        sys.exit(-1)
    del garbage[0]

    while len(garbage):
        if garbage[0] == '>':
            del garbage[0]
            return
        elif garbage[0] == '!':
            del garbage[0]
            if len(garbage) > 0:
                del garbage[0]
        else:
            del garbage[0]

    print "Error: Unclosed garbage"
    sys.exit(-1)

def score_group(group, nest=1):
    if group[0] != '{':
        print "Error: Bad group at '%s'" % (group[0])
        sys.exit(-1)
    del group[0]

    score = nest

    while len(group):
        if group[0] == '}':
            del group[0]
            return score
        elif group[0] == '{':
            score += score_group(group, nest + 1)
        elif group[0] == ',':
            del group[0]
        elif group[0] == '<':
            remove_garbage(group)
        else:
            print "Error: Unexpected input at '%s'" % (group[0])
            sys.exit(-1)

    print "Error: Unclosed input"
    sys.exit(-1)

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
    group = list(input.readline().strip())

    #print group
    score = score_group(group)
    print score

    return 0


if __name__ == "__main__":
    sys.exit(main())
