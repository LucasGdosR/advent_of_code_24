Combo_operands = [0, 1, 2, 3, 729, 0, 0, None]
A, B, C = 4, 5, 6

IP = 0
IP_inc = 2

output = []

Program = [0,1,5,4,3,0] # opcode,operand,opcode,operand...

def get_combo_operand(op):
    if op == 7:
        print('invalid')
    return Combo_operands[op]

def adv(operand):
    Combo_operands[A] //= pow(2, Combo_operands[operand])
    
def bxl(operand):
    Combo_operands[B] ^= operand

def bst(operand):
    Combo_operands[B] = Combo_operands[operand] % 8

def jnz(operand):
    global IP
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

print(output)
