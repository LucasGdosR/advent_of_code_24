Combo_operands = [0, 1, 2, 3, 2024, 0, 0, None]
A, B, C = 4, 5, 6

Program = [0,3,5,4,3,0] # opcode,operand,opcode,operand...

def get_combo_operand(operand):
    if 0 <= operand <= 6:
        return Combo_operands[operand]
    raise Exception("invalid_program")

def adv(operand):
    Combo_operands[A] //= pow(2, get_combo_operand(operand))
    
def bxl(operand):
    Combo_operands[B] ^= operand

def bst(operand):
    Combo_operands[B] = get_combo_operand(operand) % 8

def jnz(operand):
    global IP
    if Combo_operands[A] != 0:
        IP = operand - 2 # decrementing to avoid edge case

def bxc(_operand):
    Combo_operands[B] ^= Combo_operands[C]

def out(operand):
    output.append(get_combo_operand(operand) % 8)

def bdv(operand):
    Combo_operands[B] = Combo_operands[A] // pow(2, get_combo_operand(operand))

def cdv(operand):
    Combo_operands[C] = Combo_operands[A] // pow(2, get_combo_operand(operand))

instructions = [adv, bxl, bst, jnz, bxc, out, bdv, cdv]

candidate_A = 0
# Brute force: works on example, not on real input
while True:
    candidate_A += 1
    Combo_operands[A] = candidate_A
    Combo_operands[B] = 0
    Combo_operands[C] = 0
    output = []
    IP = 0
    try:
        while 0 <= IP < len(Program) - 1:
            opcode, operand = Program[IP], Program[IP + 1]
            instructions[opcode](operand)
            IP += 2
        if output == Program:
            print(candidate_A)
            break
    except:
        continue
