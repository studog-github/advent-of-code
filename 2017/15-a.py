#!/usr/bin/python

PROBLEM_NUMBER = "15-a"

import sys
import os
import argparse

class Generator:
    modval = 2147483647

    def __init__(self, factor, start_value):
        self.factor = factor
        self.value = start_value

    def next(self):
        self.value = (self.value * self.factor) % self.modval
        return self.value

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

    genA = Generator(16807, int(input.readline().strip().split()[4]))
    genB = Generator(48271, int(input.readline().strip().split()[4]))

    matches = 0
    for i in range(40000000):
        av = genA.next()
        av &= 0xFFFF
        #print format(av, '016b')
        bv = genB.next()
        bv &= 0xFFFF
        #print format(bv, '016b')
        #print format(av^bv, '016b')
        if not av ^ bv:
            matches += 1
            #print "]", matches

    print matches

    return 0


if __name__ == "__main__":
    sys.exit(main())
