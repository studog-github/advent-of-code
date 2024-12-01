#!/usr/bin/env python

import sys
import os
import argparse
from libstu import *
from pprint import pprint
from copy import deepcopy
import itertools
import math
from collections import Counter

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = THIS + "-input"

def main():
    arg_parser = argparse.ArgumentParser(prog=THIS)
    arg_parser.add_argument('-f', '--file', default=DEFAULT_INPUT,
                            help='specify the input file (default: %(default)s)', metavar='file')
    args = arg_parser.parse_args()

    # Open input file

    input_path = os.path.abspath(args.file)
    if not os.path.exists(input_path):
        print(f"{THIS}: Error: File '{args.file}' not found")
        return 1

    try:
        input = open(input_path, 'r')
    except:
        print(f"{THIS}: Error: Couldn't open file '{input_path}'")
        return 1

    # Main loop

    return 0


if __name__ == "__main__":
    sys.exit(main())
