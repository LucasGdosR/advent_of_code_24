#include <stdio.h>

#define INITIAL_STATE 1
#define RULES_NUM 1
#define LINE_LENGTH 20
#define BITS_IN_INPUT 45
#define LETTERS 26

struct state {
    unsigned char x[BITS_IN_INPUT];
    unsigned char y[BITS_IN_INPUT];
    unsigned char other[LETTERS][LETTERS][LETTERS];
};

struct rule {

};

void read_input(struct state s, struct rule rules[RULES_NUM]) {
    FILE *f = fopen("24-input-state", "r");
    char buffer[LINE_LENGTH];
    for (short i = 0; i < INITIAL_STATE; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        // save state
    }
    fclose(f);

    f = fopen("24-input-rules", "r");
    for (short i = 0; i < RULES_NUM; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        // save rule
    }
    fclose(f);
}
