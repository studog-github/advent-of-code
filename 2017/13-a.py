#!/usr/bin/python

PROBLEM_NUMBER = "13-a"

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

    firewall = []

    for line in input:
        line = map(int, line.strip().replace(':', '').split())
        #print line
        while len(firewall) < line[0]:
            firewall.append(None)
        firewall.append([line[1], 0, 1])

    #print firewall

    severity = 0
    position = 0
    while position < len(firewall):
        #print
        #print ":", position
        #print ":", firewall
        #print ":", firewall[position]
        if ((firewall[position] is not None) and (firewall[position][1] == 0)):
            severity += (position * firewall[position][0])
        for i in range(len(firewall)):
            if ((firewall[i] is not None) and (firewall[i][0] > 0)):
                firewall[i][1] += firewall[i][2]
                if ((firewall[i][1] == 0) or (firewall[i][1] == firewall[i][0] - 1)):
                    firewall[i][2] *= -1
        #print position, severity
        #print firewall
        position += 1

    print severity

    return 0


if __name__ == "__main__":
    sys.exit(main())
