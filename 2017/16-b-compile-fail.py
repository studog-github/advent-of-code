#!/usr/bin/python

PROBLEM_NUMBER = "16-b"

import sys
import os
import argparse
from libstu import *

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

    num_dancers = int(input.readline().strip())
    dancers = [chr(ord('a') + x) for x in range(0, num_dancers)]
    #print dancers
    moves = input.readline().strip().split(',')
    #print moves
    #print

    # Compile moves
    sx = deepcopy(dancers)
    p = deepcopy(dancers)
    for move in moves:
        if move[0] == 's':
            rotate(sx, int(move[1]))
        elif move[0] == 'x':
            a, b = map(int, move[1:].split('/'))
            sx[a], sx[b] = sx[b], sx[a]
        elif move[0] == 'p':
            a, b = move[1:].split('/')
            a = p.index(a)
            b = p.index(b)
            p[a], p[b] = p[b], p[a]
        else:
            print "ERROR: Unrecognized move type '%s'" % move[0]
            sys.exit(1)
    t_sx = [0] * num_dancers
    t_p = [0] * num_dancers
    for i in xrange(num_dancers):
        t_sx[i] = sx.index(dancers[i])
        t_p[i] = dancers[p.index(dancers[i])]
    print "t_sx =", t_sx
    print "t_p  =", t_p
    #print
            
    # Apply transforms
    d_tmp = deepcopy(dancers)
    #print "d_tmp:", d_tmp
    d_tmp2 = [0] * num_dancers
    for i in xrange(1):
        if i % 1000000 == 0:
            print "%10d" % i
        for i in xrange(num_dancers):
            d_tmp2[t_sx[i]] = d_tmp[i]
        #print "d_tmp2:", d_tmp2
        for i in xrange(num_dancers):
            d_tmp[d_tmp2.index(t_p[i])] = dancers[i]
        #print "d_tmp:", d_tmp
    dancers = deepcopy(d_tmp)

    print ''.join(dancers)

    return 0


if __name__ == "__main__":
    sys.exit(main())
