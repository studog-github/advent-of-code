#!/usr/bin/python

PROBLEM_NUMBER = "03-a"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def next_corner_generator():
    # First cell, center of spiral
    num = 1
    edge_len = 0
    edge_type = -1
    x = 0
    y = 0
    yield [num, edge_type, [x, y]]

    while True:
        edge_type = (edge_type + 1) % 4
        # Bottom
        if edge_type == 0:
            edge_len += 1
            x += edge_len
        # Right
        elif edge_type == 1:
            y += edge_len
        # Top
        elif edge_type == 2:
            edge_len += 1
            x -= edge_len
        # Left
        else:
            y -= edge_len
        num += edge_len
        yield [num, edge_type, [x, y]]

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

    target_num = int(input.readline())

    # Main loop

    next_corner = next_corner_generator()
    last_cell = next(next_corner)
    while True:
        next_cell = next(next_corner)
        #print "[", last_cell, ",", target_num, ",", next_cell, "]"
        if (next_cell[0] > target_num):
            break
        last_cell = next_cell

    if last_cell[0] != target_num:
        difference = target_num - last_cell[0]
        last_cell[0] = target_num

        last_cell[1] = (last_cell[1] + 1) % 4
        if last_cell[1] == 0:
            last_cell[2][0] += difference
        elif last_cell[1] == 1:
            last_cell[2][1] += difference
        elif last_cell[1] == 2:
            last_cell[2][0] -= difference
        else:
            last_cell[2][1] -= difference
        
    #print ":", last_cell, ":"
    print abs(last_cell[2][0]) + abs(last_cell[2][1])

    return 0


if __name__ == "__main__":
    sys.exit(main())
