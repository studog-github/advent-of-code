#!/usr/bin/python

PROBLEM_NUMBER = "16-a"

import sys
import os
import argparse
from libstu import *

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

MV_SPIN = 's'
MV_EXCH = 'x'
MV_PTNR = 'p'

def moves_str_to_val(s):
    if s[0] == MV_SPIN:
        return (MV_SPIN, int(s[1:]))
    elif s[0] == MV_EXCH:
        return (MV_EXCH,) + tuple(map(int, s[1:].split('/')))
    elif s[0] == MV_PTNR:
        return (MV_PTNR,) + tuple(s[1:].split('/'))
    else:
        print "ERROR: Unrecognized move '%s'" % s[0]
        sys.exit(1)

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

    moves = map(moves_str_to_val, input.readline().strip().split(','))
    #print moves

    #print dancers
    for move in moves:
        #print move
        if move[0] == MV_SPIN:
            rotate(dancers, move[1])
        elif move[0] == MV_EXCH:
            a = move[1]
            b = move[2]
            dancers[a], dancers[b] = dancers[b], dancers[a]
        else:
            a = dancers.index(move[1])
            b = dancers.index(move[2])
            dancers[a], dancers[b] = dancers[b], dancers[a]
        #print dancers

    print ''.join(dancers)

    return 0


if __name__ == "__main__":
    sys.exit(main())
