#include <stdio.h>
#include <string.h>

#define STATE_BITS 45
#define RULES_NUM 222
#define LINE_LENGTH 20
#define LETTERS 26

typedef unsigned char u8;

struct state {
    u8 x[STATE_BITS];
    u8 y[STATE_BITS];
    u8 z[STATE_BITS + 1];
    signed char other[LETTERS][LETTERS][LETTERS];
};

enum Op {
    DONE,
    AND,
    OR,
    XOR
};

struct trigram {
    char a;
    char b;
    char c;
};

struct rule {
    struct trigram i1;
    struct trigram i2;
    struct trigram o;
    enum Op op;
};

void read_input(struct state *s, struct rule rules[RULES_NUM]) {
    FILE *f = fopen("24-input-state", "r");
    char buffer[LINE_LENGTH];
    #define READ_STATE_PART(part) \
    for (u8 i = 0; i < STATE_BITS; i++) \
    { \
        fgets(buffer, LINE_LENGTH, f); \
        s->part[i] = buffer[5] - '0'; \
    }
    READ_STATE_PART(x)
    READ_STATE_PART(y)
    fclose(f);

    f = fopen("24-input-rules", "r");
    for (u8 i = 0; i < RULES_NUM; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        struct trigram i1 = *((struct trigram*)strtok(buffer, " "));
        char *op = strtok(NULL, " ");
        enum Op Op;
        switch (op[0])
        {
        case 'A': { Op = AND; break; }
        case 'O': { Op = OR ; break; }
        case 'X': { Op = XOR; break; }
        }
        struct trigram i2 = *((struct trigram*)strtok(NULL, " "));
        strtok(NULL, " "); // Skip arrow
        struct trigram o = *((struct trigram*)strtok(NULL, " "));

        rules[i] = (struct rule) { .i1=i1, .i2=i2, .op=Op, .o=o };
    }
    fclose(f);
}

u8 apply(enum Op op, u8 s1, u8 s2) {
    u8 result;
    switch (op)
    {
    case AND: { result = s1 & s2; break; }
    case OR:  { result = s1 | s2; break; }
    case XOR: { result = s1 ^ s2; break; }
    }
    return result;
}

void main(int argc, char const *argv[])
{
    struct rule rules[RULES_NUM];
    struct state state;
    memset(state.other, -1, sizeof(state.other));
    read_input(&state, rules);

    u8 rules_to_apply;
    do {
        rules_to_apply = 0;
        for (u8 i = 0; i < RULES_NUM; i++)
        {
            struct rule r = rules[i];
            if (r.op)
            {
                #define APPLY_RULE(x, y) \
                u8 i1 = (r.i1.b - '0') * 10 + (r.i1.c - '0'); \
                u8 i2 = (r.i2.b - '0') * 10 + (r.i2.c - '0'); \
                u8 result = apply(r.op, state.x[i1], state.y[i2]); \
                if (r.o.a == 'z') \
                { \
                    u8 zi = (r.o.b - '0') * 10 + (r.o.c - '0'); \
                    state.z[zi] = result; \
                } \
                else \
                { \
                    state.other[r.o.a - 'a'][r.o.b - 'a'][r.o.c - 'a'] = result; \
                } \
                r.op = DONE;

                if (r.i1.a == 'x')
                {
                    APPLY_RULE(x, y)
                }
                else if (r.i1.a == 'y')
                {
                    APPLY_RULE(y, x)
                }
                else
                {
                    char s1 = state.other[r.i1.a - 'a'][r.i1.b - 'a'][r.i1.c - 'a'];
                    char s2 = state.other[r.i2.a - 'a'][r.i2.b - 'a'][r.i2.c - 'a'];
                    if (s1 != -1 && s2 != -1)
                    {
                        u8 s = apply(r.op, s1, s2);
                        if (r.o.a == 'z')
                        {
                            u8 zi = (r.o.b - '0') * 10 + (r.o.c - '0');
                            state.z[zi] = s;
                        }
                        else
                        {
                            state.other[r.o.a - 'a'][r.o.b - 'a'][r.o.c - 'a'] = s;
                        }
                        rules[i].op = DONE;
                    }
                    else
                    {
                        rules_to_apply = 1;
                    }
                }
            }
        }
    } while (rules_to_apply);

    long total = 0;
    for (u8 i = 0; i < STATE_BITS + 1; i++)
    {
        total |= (long)state.z[i] << i;
    }
    printf("Total: %ld\n", total);
}
