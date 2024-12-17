#include <stdio.h>
#include <stdlib.h>

#define A 4
#define B 5
#define C 6
#define IP_INC 2

__uint64_t combo_operands[] = { 0, 1, 2, 3, 729, 0, 0 };
__int8_t IP = 0;
__uint8_t output[9];
__uint8_t output_i = 0;

__uint64_t get_combo_operand(__uint8_t operand) {
    if (0 <= operand && operand <= 6) {
        return combo_operands[operand];
    }
    exit(operand);
}

void adv(__uint8_t operand) {
    combo_operands[A] /= 1 << get_combo_operand(operand);
}
    
void bxl(__uint8_t operand) {
    combo_operands[B] ^= operand;
}

void bst(__uint8_t operand) {
    combo_operands[B] = get_combo_operand(operand) & 7;
}

void jnz(__uint8_t operand) {
    IP = combo_operands[A] ? operand - 2 : IP;
}

void bxc() {
    combo_operands[B] ^= combo_operands[C];
}

void out(__uint8_t operand) {
    output[output_i++] = get_combo_operand(operand) & 7;
}

void bdv(__uint8_t operand) {
    combo_operands[B] = combo_operands[A] / (1 << get_combo_operand(operand));
}

void cdv(__uint8_t operand) {
    combo_operands[C] = combo_operands[A] / (1 << get_combo_operand(operand));
}

void main(int argc, char const *argv[])
{
    __uint8_t program[] = { 0,1,5,4,3,0 };
    while ((0 <= IP)  && (IP < (sizeof(program) / sizeof(__uint8_t) - 1))) {
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
    printf("Output: ");
    for (__uint8_t i = 0; i < output_i; i++)
    {
        printf("%hhd,", output[i]);
    }
    printf("\n");
}
