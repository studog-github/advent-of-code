class Computer:
    # Machine operations

    # Parameter modes:
    # '0' - Position
    # '1' - Immediate
    # '2' - Relative

    # Refactor parameter modes

    def addition(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        r2 = self.program[self.ip + 2]
        if self.opmode[1] == '0':
            r2 = self.program[r2]
        elif self.opmode[1] == '2':
            r2 = self.program[r2 + self.relbase]
        s1 = self.program[self.ip + 3]
        self.program[s1] = r1 + r2

    def multiplication(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        r2 = self.program[self.ip + 2]
        if self.opmode[1] == '0':
            r2 = self.program[r2]
        elif self.opmode[1] == '2':
            r2 = self.program[r2 + self.relbase]
        s1 = self.program[self.ip + 3]
        self.program[s1] = r1 * r2

    def poke(self):
        r1 = self.poke_input()
        #print(f'-= poke got {r1}')
        s1 = self.program[self.ip + 1]
        self.program[s1] = r1
        
    def peek(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        #print(f'= peek returns {r1}')
        self.peek_output(r1)
        
    def jumpiftrue(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        r2 = self.program[self.ip + 2]
        if self.opmode[1] == '0':
            r2 = self.program[r2]
        elif self.opmode[1] == '2':
            r2 = self.program[r2 + self.relbase]
        if r1 != 0:
            self.ip = r2 - self.oplen

    def jumpiffalse(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        r2 = self.program[self.ip + 2]
        if self.opmode[1] == '0':
            r2 = self.program[r2]
        elif self.opmode[1] == '2':
            r2 = self.program[r2 + self.relbase]
        if r1 == 0:
            self.ip = r2 - self.oplen

    def lessthan(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        r2 = self.program[self.ip + 2]
        if self.opmode[1] == '0':
            r2 = self.program[r2]
        elif self.opmode[1] == '2':
            r2 = self.program[r2 + self.relbase]
        s1 = self.program[self.ip + 3]
        if r1 < r2:
            self.program[s1] = 1
        else:
            self.program[s1] = 0

    def equals(self):
        r1 = self.program[self.ip + 1]
        if self.opmode[0] == '0':
            r1 = self.program[r1]
        elif self.opmode[0] == '2':
            r1 = self.program[r1 + self.relbase]
        r2 = self.program[self.ip + 2]
        if self.opmode[1] == '0':
            r2 = self.program[r2]
        elif self.opmode[1] == '2':
            r2 = self.program[r2 + self.relbase]
        s1 = self.program[self.ip + 3]
        if r1 == r2:
            self.program[s1] = 1
        else:
            self.program[s1] = 0

    def chrelbase(self):
        r1 = self.program[self.ip + 1]
        self.relbase += r1

    OP_ADD = 1
    OP_MULT = 2
    OP_POKE = 3
    OP_PEEK = 4
    OP_JMPT = 5
    OP_JMPF = 6
    OP_LT = 7
    OP_EQ = 8
    OP_CHRELB = 9
    OP_HALT = 99

    _OP_FN = 0
    _OP_NINST = 1

    opcodes = {
        OP_ADD: (addition, 4),
        OP_MULT: (multiplication, 4),
        OP_POKE: (poke, 2),
        OP_PEEK: (peek, 2),
        OP_JMPT: (jumpiftrue, 3),
        OP_JMPF: (jumpiffalse, 3),
        OP_LT: (lessthan, 4),
        OP_EQ: (equals, 4),
        OP_CHRELB: (chrelbase, 2),
        OP_HALT: (None, 1),
    }

    STATE_INITED = 0
    STATE_RESET = 1
    STATE_RUNNING = 2
    STATE_PAUSED = 3
    STATE_HALTED = 4

    def poke_user_input(self):
        return int(raw_input('poke needs a number: '))

    def peek_user_output(self, v):
        print(f'peek: {v}')

    # __repr()__ and __str()__ ?

    #
    # Regular members and methods
    #

    def __init__(self, program, fin=None, fout=None):
        self.original_program = program
        self.poke_input = fin if fin else self.poke_user_input
        self.peek_output = fout if fout else self.peek_user_output
        self.state = self.STATE_INITED

    # Big memory:
    # - dict for memory locations
    # - parse program into memory

    def reset(self):
        self.program = list(self.original_program)
        self.ip = 0
        self.relbase = 0
        self.state = self.STATE_RESET

    def run(self):
        self.state = self.STATE_RUNNING
        while True:
            print(f'program {self.program}')
            self.opcode = self.program[self.ip] % 100
            if self.opcode not in self.opcodes:
                print(f'ERROR: Unknown opcode {self.opcode} at index {self.ip}, halted')
                self.state = self.STATE_HALTED
                return
            self.opmode = str(self.program[self.ip])[:-2][::-1]
            self.oplen = self.opcodes[self.opcode][self._OP_NINST]
            self.opmode += '0' * (self.oplen - 1 - len(self.opmode))
            print(f"ip: {self.ip} {self.program[self.ip:self.ip+self.oplen]} mode: '{self.opmode}' relbase: {self.relbase}")
            self.opfunc = self.opcodes[self.opcode][self._OP_FN]
            if self.opfunc is None:
                self.state = self.STATE_HALTED
                return
            self.opfunc(self)
            self.ip += self.oplen
