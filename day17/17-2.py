Combo_operands = [0, 1, 2, 3, 2024, 0, 0, None]
A, B, C = 4, 5, 6
IP_inc = 2
Program = [0,3,5,4,3,0] # opcode,operand,opcode,operand...

def execute_program(a):
    Combo_operands[A] = a
    Combo_operands[B] = 0
    Combo_operands[C] = 0
    IP = 0
    output = []

    def adv(operand):
        Combo_operands[A] //= pow(2, Combo_operands[operand])
        
    def bxl(operand):
        Combo_operands[B] ^= operand

    def bst(operand):
        Combo_operands[B] = Combo_operands[operand] % 8

    def jnz(operand):
        nonlocal IP
        if Combo_operands[A] != 0:
            IP = operand - 2 # decrementing to avoid edge case

    def bxc(_operand):
        Combo_operands[B] ^= Combo_operands[C]

    def out(operand):
        output.append(Combo_operands[operand] % 8)

    def bdv(operand):
        Combo_operands[B] = Combo_operands[A] // pow(2, Combo_operands[operand])

    def cdv(operand):
        Combo_operands[C] = Combo_operands[A] // pow(2, Combo_operands[operand])

    instructions = [adv, bxl, bst, jnz, bxc, out, bdv, cdv]

    while 0 <= IP < len(Program):
        opcode, operand = Program[IP], Program[IP + 1]
        instructions[opcode](operand)
        IP += IP_inc
    
    return output

max_a = (1 << (len(Program) * 3)) - 1
min_a = (max_a >> 3) + 1
candidates = [min_a]

for i in range(1, len(Program) + 1):
    new_candidates = []
    step = 1 << ((len(Program) - i) * 3)
    for c in candidates:
        for a_register in range(c, c + 7 * step + 1, step):
            o = execute_program(a_register)
            if o[-i] == Program[-i]:
                new_candidates.append(a_register)
    candidates = new_candidates

print(min(candidates))
