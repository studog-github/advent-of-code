#!/usr/bin/python

PROBLEM_NUMBER = "07-a"

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

    nodes = {}
    for line in input:
        line = line.strip().split()
        if len(line) <= 3:
            children = []
        else:
            children = [x.strip(",") for x in line[3:]]
        nodes[line[0]] = [int(line[1].strip("()")), None, children]
    # Fill in parents
    for node in nodes:
        for child in nodes[node][2]:
            nodes[child][1] = node
    for node in nodes:
        if nodes[node][1] == None:
            break

    #print nodes
    print node

    return 0


if __name__ == "__main__":
    sys.exit(main())
