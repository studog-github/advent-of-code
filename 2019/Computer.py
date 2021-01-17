from libstu import GrowingList

class Computer:
    # Machine operations

    OPMODE_POS = '0'
    OPMODE_IMM = '1'
    OPMODE_REL = '2'
    OPMODES = OPMODE_POS + OPMODE_IMM + OPMODE_REL

    def _get_roperand(self, index):
        operand = self.program[self.ip + index]
        if self.opmode[index] == self.OPMODE_POS:
            operand = self.program[operand]
        elif self.opmode[index] == self.OPMODE_REL:
            operand = self.program[operand + self.relbase]
        return operand

    def _get_loperand(self, index):
        operand = self.program[self.ip + index]
        if self.opmode[index] == self.OPMODE_REL:
            operand += self.relbase
        return operand

    def addition(self):
        r1 = self._get_roperand(1)
        r2 = self._get_roperand(2)
        s1 = self._get_loperand(3)
        self.program[s1] = r1 + r2

    def multiplication(self):
        r1 = self._get_roperand(1)
        r2 = self._get_roperand(2)
        s1 = self._get_loperand(3)
        self.program[s1] = r1 * r2

    def poke(self):
        r1 = self.poke_input()
        s1 = self._get_loperand(1)
        self.program[s1] = r1

    def peek(self):
        r1 = self._get_roperand(1)
        self.peek_output(r1)

    def jumpiftrue(self):
        r1 = self._get_roperand(1)
        r2 = self._get_roperand(2)
        if r1 != 0:
            self.ip = r2 - self.oplen

    def jumpiffalse(self):
        r1 = self._get_roperand(1)
        r2 = self._get_roperand(2)
        if r1 == 0:
            self.ip = r2 - self.oplen

    def lessthan(self):
        r1 = self._get_roperand(1)
        r2 = self._get_roperand(2)
        s1 = self._get_loperand(3)
        if r1 < r2:
            self.program[s1] = 1
        else:
            self.program[s1] = 0

    def equals(self):
        r1 = self._get_roperand(1)
        r2 = self._get_roperand(2)
        s1 = self._get_loperand(3)
        if r1 == r2:
            self.program[s1] = 1
        else:
            self.program[s1] = 0

    def chrelbase(self):
        r1 = self._get_roperand(1)
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
        return int(input('poke needs a number: '))

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

    def reset(self):
        self.program = GrowingList(self.original_program, fill_value=0)
        self.ip = 0
        self.relbase = 0
        self.state = self.STATE_RESET

    def run(self):
        self.state = self.STATE_RUNNING
        while True:
            #print(f'program {self.program}')
            self.opcode = self.program[self.ip] % 100
            if self.opcode not in self.opcodes:
                print(f'ERROR: Unknown opcode {self.opcode} at index {self.ip}, halted')
                self.state = self.STATE_HALTED
                return
            self.opmode = str(self.program[self.ip])[:-2][::-1]
            if set(self.opmode) - set(self.OPMODES):
                print(f'ERROR: Unknown opmode {self.opmode} at index {self.ip}, halted')
                self.state = self.STATE_HALTED
                return
            self.oplen = self.opcodes[self.opcode][self._OP_NINST]
            self.opmode = '.' + self.opmode + self.OPMODE_POS * (self.oplen - 1 - len(self.opmode))
            #print(f"ip: {self.ip} {self.program[self.ip:self.ip+self.oplen]} mode: '{self.opmode}' relbase: {self.relbase}")
            self.opfunc = self.opcodes[self.opcode][self._OP_FN]
            if self.opfunc is None:
                self.state = self.STATE_HALTED
                return
            self.opfunc(self)
            self.ip += self.oplen
