#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 48
#define INPUT_SIZE 850
#define MAX_OPERANDS 13
#define SPACE_FOR_STORING_LENGTH 1

int can_combine(long tv, long *ops, long acc, long ops_left);
long concat(long big, long small);
void read_input();

char buffer[LINE_LENGTH];
long test_values[INPUT_SIZE];
long operands[INPUT_SIZE][MAX_OPERANDS + SPACE_FOR_STORING_LENGTH];

void main(int argc, char const *argv[]) {
    read_input();
    unsigned long calibration_result = 0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        long tv = test_values[i], *ops = operands[i];
        calibration_result += tv * can_combine(tv, ops + 1, 0, ops[0]);
    }
    printf("Total calibration result: %ld\n", calibration_result);
}

int can_combine(long tv, long *ops, long acc, long ops_left) {
    if (ops_left == 0) {
        return tv == acc;
    }
    return can_combine(tv, ops + 1, acc + (*ops),      ops_left - 1)
        || can_combine(tv, ops + 1, acc * (*ops),      ops_left - 1)
        || can_combine(tv, ops + 1, concat(acc, *ops), ops_left - 1);
}

long concat(long big, long small) {
    return big * pow(10., (double) ((int) log10(small) + 1)) + small;
}

void read_input() {
    FILE *fptr = fopen("7-input", "r");
    for (int i = 0; i < INPUT_SIZE; i++) {
        fgets(buffer, LINE_LENGTH, fptr);
        char *token, *ep, *rest = buffer;
        token = strsep(&rest, ":");
        rest++;
        test_values[i] = strtol(token, &ep, 10);

        int j = 0;
        while (token = strsep(&rest, " ")) {
            operands[i][++j] = strtol(token, &ep, 10);
        }
        operands[i][0] = j;
    }
    fclose(fptr);
}
