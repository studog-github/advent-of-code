#!/usr/bin/python

PROBLEM_NUMBER = "18-b"

import sys
import os
import argparse
from libstu import *

THIS = os.path.basename(sys.argv[0])
DEFAULT_INPUT = PROBLEM_NUMBER + "-input"

def run_machine(machine, insts):
    mach_id = machine['p']
    num_insts = len(insts)

    while machine['ip'] >= 0 and machine['ip'] < num_insts:
        inst = insts[machine['ip']]
        #print "[%d] %s" % (mach_id, machine)
        #print "[%d] %d: %s" % (mach_id, machine['ip'], inst)
        if inst[0] == 'snd':
            machine['sendcount'] += 1
            yield machine[inst[1]]
        elif inst[0] == 'set':
            machine[inst[1]] = machine[inst[2]]
        elif inst[0] == 'add':
            machine[inst[1]] += machine[inst[2]]
        elif inst[0] == 'mul':
            machine[inst[1]] *= machine[inst[2]]
        elif inst[0] == 'mod':
            machine[inst[1]] = machine[inst[1]] % machine[inst[2]]
        elif inst[0] == 'rcv':
            val = (yield None)
            while val is None:
                val = (yield None)
            machine[inst[1]] = val
        elif inst[0] == 'jgz':
            if machine[inst[1]] > 0:
                machine['ip'] += machine[inst[2]]
                continue
        machine['ip'] += 1

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

    machine = {}

    insts = []
    for line in input:
        line = line.strip().split()
        if line[0] not in ['snd', 'set', 'add', 'mul', 'mod', 'rcv', 'jgz']:
            print "Error: Unrecognised instruction '%s'" % line[0]
            sys.exit(1)
        try:
            line[1] = int(line[1])
        except ValueError:
            pass
        if isinstance(line[1], int):
            machine[line[1]] = line[1]
        else:
            machine[line[1]] = 0
        if len(line) == 3:
            try:
                line[2] = int(line[2])
            except ValueError:
                pass
            if isinstance(line[2], int):
                machine[line[2]] = line[2]
            else:
                machine[line[2]] = 0
        if line[0] in ['set', 'add', 'mul', 'mod', 'rcv'] and isinstance(line[1], int):
            print "Error: '%s' expected register, got '%s'" % (line[0], line[1])
            sys.ext(1)
        #print line
        insts.append(tuple(line))
    num_insts = len(insts)
    print "num insts:", num_insts
    print insts
    print

    machine['ip'] = 0
    machine['sendcount'] = 0
    #print machine
    #print

    # Run two machines with IDs
    mach0 = deepcopy(machine)
    mach0['p'] = 0
    mach1 = deepcopy(machine)
    mach1['p'] = 1
    #print "mach0:", mach0
    #print "mach1:", mach1
    #print

    # Init m0
    m0 = run_machine(mach0, insts)
    m0q = []
    val = m0.next()
    if val is not None:
        m0q.append(val)
    # Init m1
    m1 = run_machine(mach1, insts)
    m1q = []
    val = m1.next()
    if val is not None:
        m1q.append(val)
    print "m0q:", m0q
    print "m1q:", m1q
    print
    while True:
        #print "[%d] %s" % (mach0['p'], insts[mach0['ip']][0])
        if insts[mach0['ip']][0] == 'snd':
            val = m0.next()
            if val is not None:
                m0q.append(val)
        elif len(m1q) > 0:
            val = m0.send(m1q.pop(0))
            if val is not None:
                m0q.append(val)
        #print "[%d] %s" % (mach1['p'], insts[mach1['ip']][0])
        if insts[mach1['ip']][0] == 'snd':
            val = m1.next()
            if val is not None:
                m1q.append(val)
        elif len(m0q) > 0:
            val = m1.send(m0q.pop(0))
            if val is not None:
                m1q.append(val)
        print "m0q:", m0q
        print "m1q:", m1q
        print
        if (insts[mach0['ip']][0] == 'rcv' and len(m1q) == 0 and
                insts[mach1['ip']][0] == 'rcv' and len(m0q) == 0):
            break

    print "ans:", mach1['sendcount']

    return 0


if __name__ == "__main__":
    sys.exit(main())
