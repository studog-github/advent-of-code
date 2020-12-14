class computer:
    # Machine operations
    def addition(self, mode):
        num_instns = 4
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        r2 = self.program[self.ip + 2]
        if mode[1] == '0':
            r2 = self.program[r2]
        s1 = self.program[self.ip + 3]
        self.program[s1] = r1 + r2
        self.ip += num_instns

    def multiplication(self, mode):
        num_instns = 4
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        r2 = self.program[self.ip + 2]
        if mode[1] == '0':
            r2 = self.program[r2]
        s1 = self.program[self.ip + 3]
        self.program[s1] = r1 * r2
        self.ip += num_instns

    def poke(self, mode):
        num_instns = 2
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.poke_input()
        #print(f'-= poke got {r1}')
        s1 = self.program[self.ip + 1]
        self.program[s1] = r1
        self.ip += num_instns
        
    def peek(self, mode):
        num_instns = 2
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        #print(f'= peek returns {r1}')
        self.peek_output(r1)
        self.ip += num_instns
        
    def jumpiftrue(self, mode):
        num_instns = 3
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        r2 = self.program[self.ip + 2]
        if mode[1] == '0':
            r2 = self.program[r2]
        if r1 != 0:
            self.ip = r2
        else:
            self.ip += num_instns

    def jumpiffalse(self, mode):
        num_instns = 3
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        r2 = self.program[self.ip + 2]
        if mode[1] == '0':
            r2 = self.program[r2]
        if r1 == 0:
            self.ip = r2
        else:
            self.ip += num_instns

    def lessthan(self, mode):
        num_instns = 4
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        r2 = self.program[self.ip + 2]
        if mode[1] == '0':
            r2 = self.program[r2]
        s1 = self.program[self.ip + 3]
        if r1 < r2:
            self.program[s1] = 1
        else:
            self.program[s1] = 0
        self.ip += num_instns

    def equals(self, mode):
        num_instns = 4
        mode += '0' * (num_instns - 1 - len(mode))
        r1 = self.program[self.ip + 1]
        if mode[0] == '0':
            r1 = self.program[r1]
        r2 = self.program[self.ip + 2]
        if mode[1] == '0':
            r2 = self.program[r2]
        s1 = self.program[self.ip + 3]
        if r1 == r2:
            self.program[s1] = 1
        else:
            self.program[s1] = 0
        self.ip += num_instns

    opcodes = {
        1: addition,
        2: multiplication,
        3: poke,
        4: peek,
        5: jumpiftrue,
        6: jumpiffalse,
        7: lessthan,
        8: equals,
        99: None,
    }

    STATE_INITED = 0
    STATE_RUNNING = 1
    STATE_PAUSED = 2
    STATE_HALTED = 3

    def poke_user_input(self):
        return int(raw_input("poke needs a number: "))

    def peek_user_output(self, v):
        print(f'peek: {v}')

    # Regular members and methods
    def __init__(self, program, fin=None, fout=None):
        self.original_program = program
        self.poke_input = fin if fin else self.poke_user_input
        self.peek_output = fout if fout else self.peek_user_output

    def reset(self):
        self.program = list(self.original_program)
        self.ip = 0
        self.state = self.STATE_INITED

    def run(self):
        self.state = self.STATE_RUNNING
        while True:
            print(f'program {self.program}')
            p_opcode = self.program[self.ip] % 100
            p_opmode = str(self.program[self.ip])[:-2][::-1]
            print(f"ip: {self.ip} {self.program[self.ip:self.ip+4]} mode: '{p_opmode}' relbase: {self.relbase}")
            if p_opcode not in self.opcodes:
                print(f'ERROR: Unknown opcode {p_opcode} at index {self.ip}, halted')
                self.state = self.STATE_HALTED
                return
            elif self.opcodes[p_opcode] is None:
                self.state = self.STATE_HALTED
                return
            self.opcodes[p_opcode](self, p_opmode)
            if self.opcodes[p_opcode] == computer.__dict__['peek']:
                self.state = self.STATE_PAUSED
                return
