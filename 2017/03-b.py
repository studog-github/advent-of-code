#!/usr/bin/python

PROBLEM_NUMBER = "03-b"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

# Cell [[x, y], edge_type, edge_size]

def ulam():
    # First cell, center of spiral
    edge_len = 0
    edge_type = -1
    x = 0
    y = 0
    yield [(x, y), edge_type, edge_len]

    while True:
        edge_type = (edge_type + 1) % 4
        if edge_type == 0 or edge_type == 2:
            edge_len += 1

        i = 0
        while i < edge_len:
            # Bottom
            if edge_type == 0:
                x += 1
            # Right
            elif edge_type == 1:
                y += 1
            # Top
            elif edge_type == 2:
                x -= 1
            # Left
            else:
                y -= 1
            yield [(x, y), edge_type, edge_len]
            i += 1

def boxsum(cell, grid):
    sum = 0
    x = cell[0][0]
    y = cell[0][1]

    try:
        sum += grid[(x+1,y+0)]
    except KeyError:
        pass
    try:
        sum += grid[(x+1,y+1)]
    except KeyError:
        pass
    try:
        sum += grid[(x+0,y+1)]
    except KeyError:
        pass
    try:
        sum += grid[(x-1,y+1)]
    except KeyError:
        pass
    try:
        sum += grid[(x-1,y+0)]
    except KeyError:
        pass
    try:
        sum += grid[(x-1,y-1)]
    except KeyError:
        pass
    try:
        sum += grid[(x+0,y-1)]
    except KeyError:
        pass
    try:
        sum += grid[(x+1,y-1)]
    except KeyError:
        pass

    return sum

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

    grid = {}
    ulam_gen = ulam()

    cell = next(ulam_gen)
    grid[cell[0]] = 1
    #print cell, "=>", grid[cell[0]]

    while True:
        cell = next(ulam_gen)
        grid[cell[0]] = boxsum(cell, grid)
        #print cell, "=>", grid[cell[0]]
        if grid[cell[0]] > target_num:
            break

    print grid[cell[0]]

    return 0


if __name__ == "__main__":
    sys.exit(main())
