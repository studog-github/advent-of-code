#!/usr/bin/python

PROBLEM_NUMBER = "13-b"

import sys
import os
import argparse
from copy import deepcopy

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
    firewall_len = len(firewall)

    delay = 0
    while True:
        print ": d", delay
        tmp_fw = deepcopy(firewall)
        caught = False
        position = 0
        while position < firewall_len:
            if ((tmp_fw[position] is not None) and (tmp_fw[position][1] == 0)):
                caught = True
                break
            position += 1
            for i in range(firewall_len):
                if (tmp_fw[i] is not None):
                    tmp_fw[i][1] += tmp_fw[i][2]
                    if ((tmp_fw[i][1] == 0) or (tmp_fw[i][1] == tmp_fw[i][0] - 1)):
                        tmp_fw[i][2] *= -1
        if not caught:
            break
        for i in range(firewall_len):
            if (firewall[i] is not None):
                firewall[i][1] += firewall[i][2]
                if ((firewall[i][1] == 0) or (firewall[i][1] == firewall[i][0] - 1)):
                    firewall[i][2] *= -1
        delay += 1

    print "delay:", delay

    return 0


if __name__ == "__main__":
    sys.exit(main())
