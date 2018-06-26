#!/usr/bin/python

PROBLEM_NUMBER = "10-b"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def do_hash(params):
    l = params[0]
    lengths = params[1]
    start = params[2]
    skip_size = params[3]
    list_len = len(l)

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

    params[1] = l
    params[1] = lengths
    params[2] = start
    params[3] = skip_size

def do_densify(l):
    sl = [0] * 16

    for i in range(0, 256/16):
        start = i * 16
        for j in range(start, start + 16):
            sl[i] ^= l[j]

    return sl

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

    l = range(256)

    lengths = map(ord, input.readline().strip())
    # As ordained by the puzzle
    lengths += [17, 31, 73, 47, 23]
    print lengths

    skip_size = 0
    start = 0

    params = [l, lengths, 0, 0]
    for i in range(64):
        do_hash(params)
    sl = do_densify(l)
    print sl
    sl_str = ''.join('%02x' % i for i in sl)

    print sl_str

    return 0


if __name__ == "__main__":
    sys.exit(main())
