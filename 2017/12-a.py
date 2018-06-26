#!/usr/bin/python

PROBLEM_NUMBER = "12-a"

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

    graph = {}

    # Main loop
    for line in input:
        line = map(int, line.strip().replace("<-> ", "").replace(",", "").split())
        #print line
        graph[line[0]] = line[1:]
    #print graph

    seen = {}
    to_visit = [0]
    while len(to_visit):
        print to_visit
        node = to_visit.pop(0)
        if node in seen:
            continue
        seen[node] = 1
        to_visit += graph[node]

    print seen
    print len(seen)

    return 0


if __name__ == "__main__":
    sys.exit(main())
