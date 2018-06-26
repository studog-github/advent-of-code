#!/usr/bin/python

PROBLEM_NUMBER = "14-b"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def knot_hash(key):
    lengths = map(ord, key)
    lengths += [17, 31, 73, 47, 23]

    list_len = 256
    l = range(list_len)
    skip_size = 0
    start = 0

    for i in range(64):
        for length in lengths:
            end = start + length
            if length <= 1:
                pass
            elif end < list_len:
                l[start:end] = reversed(l[start:end])
            else:
                l *= 2
                l[start:end] = reversed(l[start:end])
                l[:end - list_len] = l[list_len:end]
                del l[list_len:]
            start = (start + length + skip_size) % list_len
            skip_size += 1

    sl = [0] * 16
    for i in range(0, 256/16):
        start = i * 16
        for j in range(start, start + 16):
            sl[i] ^= l[j]

    h = int(''.join('%02x' % i for i in sl), 16)

    return h

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

    key = input.readline().strip()
    key += '-'
    grid = []
    for i in range(128):
        k = key + str(i)
        h = knot_hash(k)
        grid.append([[int(x), 0] for x in format(h, '0128b')])
    #print grid

    S_FR_US = 0
    FREE = 0
    USED = 1
    S_REGION = 1
    NO_REGION = 0

    region = 0
    to_visit = []
    for i in range(128):
        for j in range(128):
            #print "Loop (%d, %d)%s" % (i, j, grid[i][j])
            if grid[i][j][S_FR_US] == FREE or grid[i][j][S_REGION] != NO_REGION:
                continue
            region += 1
            #print
            #print "Region:", region
            to_visit += [(i, j)]
            while len(to_visit):
                #print "vlist:", to_visit
                (ii, jj) = to_visit.pop(0)
                #print "Visit (%d, %d)%s" % (ii, jj, grid[ii][jj])
                if grid[ii][jj][S_REGION] != NO_REGION and grid[ii][jj][S_REGION] != region:
                    print("ERROR: node[%d, %d] already in region %d (creating region %d)" %
                                (ii, jj, grid[ii][jj][S_REGION], region))
                    sys.exit(1)
                grid[ii][jj][S_REGION] = region
                #print "add: (%d, %d)%s" % (ii, jj, grid[ii][jj])
                if ii > 0 and grid[ii-1][jj][S_FR_US] == USED and grid[ii-1][jj][S_REGION] != region:
                    #print "queue (%d, %d)%s" % (ii-1, jj, grid[ii-1][jj])
                    to_visit += [(ii-1, jj)]
                if jj < 127 and grid[ii][jj+1][S_FR_US] == USED and grid[ii][jj+1][S_REGION] != region:
                    #print "queue (%d, %d)%s" % (ii, jj+1, grid[ii][jj+1])
                    to_visit += [(ii, jj+1)]
                if ii < 127 and grid[ii+1][jj][S_FR_US] == USED and grid[ii+1][S_REGION] != region:
                    #print "queue (%d, %d)%s" % (ii+1, jj, grid[ii+1][jj])
                    to_visit += [(ii+1, jj)]
                if jj > 0 and grid[ii][jj-1][S_FR_US] == USED and grid[ii][jj-1][S_REGION] != region:
                    #print "queue (%d, %d)%s" % (ii, jj-1, grid[ii][jj-1])
                    to_visit += [(ii, jj-1)]

    print "regions:", region

    return 0


if __name__ == "__main__":
    sys.exit(main())
