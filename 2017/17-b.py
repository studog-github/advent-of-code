#!/usr/bin/python

PROBLEM_NUMBER = "17-b"

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

    step_size = int(input.readline().strip())
    #print "step_size:", step_size

    num_nums = 50000000
    #circ = [0]
    #circ = [0] * num_nums
    circ = [0, 0]
    clen = 1
    pos = 0

    print "circ:", circ
    #print "circ:", circ[0:clen], "circ:", circ
    #print "pos:", pos, "clen:", clen
    val = 1
    while val < num_nums:
        #if val % 10000 == 0:
            #print val
        pos = (pos + step_size) % clen
        #circ.insert(pos + 1, val)
        pos += 1
        #if pos != clen:
            #circ[pos+1:clen+1] = circ[pos:clen]
        #circ[pos] = val
        if pos == 1:
            circ[pos] = val
        val += 1
        clen += 1
        #if clen > 30:
            #print "circ:", circ[:30]
        #else:
            #print "circ:", circ
        #print "circ:", circ[0:clen], "circ:", circ
        #print "pos:", pos, "clen:", clen
    print "pos:", pos, "clen:", clen
    print "circ:", circ
    print "ans:", circ[1]
    #print "circ:", circ[pos-3:pos+4]
    #print "ans:", circ[pos+1]
    

    return 0


if __name__ == "__main__":
    sys.exit(main())
