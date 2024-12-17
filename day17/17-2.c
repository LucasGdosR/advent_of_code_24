#include <stdio.h>

#define A 4
#define B 5
#define C 6
#define IP_INC 2
#define len(program) (sizeof(program) / sizeof(__uint8_t))

struct arr {
    __uint64_t elements[32];
    __uint8_t size;
};

__uint64_t combo_operands[] = { 0, 1, 2, 3, 2024, 0, 0 };
__uint8_t program[] = { 0,3,5,4,3,0 };
__int8_t IP = 0;
__uint8_t output[len(program)];
__uint8_t output_i;

void adv(__uint8_t operand) {
    combo_operands[A] /= 1ULL << combo_operands[operand];
}
    
void bxl(__uint8_t operand) {
    combo_operands[B] ^= operand;
}

void bst(__uint8_t operand) {
    combo_operands[B] = combo_operands[operand] & 0b111;
}

void jnz(__uint8_t operand) {
    IP = combo_operands[A] ? operand - IP_INC : IP;
}

void bxc() {
    combo_operands[B] ^= combo_operands[C];
}

void out(__uint8_t operand) {
    output[output_i++] = combo_operands[operand] & 0b111;
}

void bdv(__uint8_t operand) {
    combo_operands[B] = combo_operands[A] / (1ULL << combo_operands[operand]);
}

void cdv(__uint8_t operand) {
    combo_operands[C] = combo_operands[A] / (1ULL << combo_operands[operand]);
}

void execute_program(__uint64_t a_register) {
    combo_operands[A] = a_register;
    combo_operands[B] = 0;
    combo_operands[C] = 0;
    IP = 0;
    output_i = 0;

    while ((0 <= IP)  && (IP < len(program) - 1)) {
        __uint8_t opcode = program[IP], operand = program[IP + 1];
        switch (opcode)
        {
        case 0: { adv(operand); break; }
        case 1: { bxl(operand); break; }
        case 2: { bst(operand); break; }
        case 3: { jnz(operand); break; }
        case 4: { bxc(       ); break; }
        case 5: { out(operand); break; }
        case 6: { bdv(operand); break; }
        case 7: { cdv(operand); break; }
        default: printf("invalid opcode: %hhd\n", opcode);
        }
        IP += IP_INC;
    }
}

void main(int argc, char const *argv[])
{
    __uint64_t max_a = (1ULL << (len(program) * 3)) - 1;
    __uint64_t min_a = (max_a >> 3) + 1;
    struct arr candidates = (struct arr) { .elements = { min_a }, .size = 1 };

    for (__uint8_t i = 1; i <= len(program); i++)
    {
        struct arr new_candidates;
        new_candidates.size = 0;
        __uint64_t step = 1ULL << ((len(program) - i) * 3);
        for (__uint8_t j = 0; j < candidates.size; j++)
        {
            __uint64_t c = candidates.elements[j];
            for (__uint64_t a_register = c; a_register < c + 7 * step + 1; a_register += step)
            {
                execute_program(a_register);
                if (output[len(program) - i] == program[len(program) - i])
                {
                    new_candidates.elements[new_candidates.size++] = a_register;
                }
            }
        }
        candidates = new_candidates;
    }
    printf("Register A uncorrupted value: %ld\n", candidates.elements[0]);
}
