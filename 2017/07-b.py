#!/usr/bin/python

PROBLEM_NUMBER = "07-b"

import sys
import os
import argparse

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def print_tree(node, nodes, indent=""):
    print indent, node, nodes[node]
    for child in nodes[node][2]:
        print_tree(child, nodes, indent + "  ")

# If a tower has only 2 children and they mismatch, it's not possible to tell
# which of the two children is the wrong weight and which is the right weight
# Therefore, we must assume that the puzzle always has a branching factor of
# at least 3.
def balance_tree(node, nodes):
    if len(nodes[node][2]) == 0:
        nodes[node].append(0)
        return 0
    child_sums = []
    for child in nodes[node][2]:
        correction_factor = balance_tree(child, nodes)
        if correction_factor:
            return correction_factor
        child_sums.append([child, nodes[child][0] + nodes[child][3]])
    child_sums.sort(key=lambda child: child[1])
    #print "=>", child_sums
    # There's only one wrong value, so it's either at the start or the end
    difference = child_sums[1][1] - child_sums[0][1]
    if difference:
        return nodes[child_sums[0][0]][0] + difference
    difference = child_sums[-1][1] - child_sums[-2][1]
    if difference:
        return nodes[child_sums[-1][0]][0] - difference
    nodes[node].append(sum([y for [x, y] in child_sums]))
    return 0

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

    nodes = {}
    for line in input:
        line = line.strip().split()
        if len(line) <= 3:
            children = []
        else:
            children = [x.strip(",") for x in line[3:]]
        nodes[line[0]] = [int(line[1].strip("()")), None, children]
    # Fill in parents
    for node in nodes:
        for child in nodes[node][2]:
            nodes[child][1] = node
    for node in nodes:
        if nodes[node][1] == None:
            break

    correction_factor = balance_tree(node, nodes)
    #print_tree(node, nodes)
    print correction_factor

    return 0


if __name__ == "__main__":
    sys.exit(main())
