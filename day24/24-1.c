#include <stdio.h>
#include <string.h>

#define STATE_BITS 45
#define RULES_NUM 222
#define LINE_LENGTH 20
#define LETTERS 26

#define CHAR_TO_I(i_or_o) ((r.i_or_o.b - '0') * 10 + (r.i_or_o.c - '0'))
#define TRI_TO_I(i_or_o) [r.i_or_o.a - 'a'][r.i_or_o.b - 'a'][r.i_or_o.c - 'a']

typedef unsigned char u8;
typedef signed   char i8;

struct state {
    i8 x[STATE_BITS];
    i8 y[STATE_BITS];
    i8 z[STATE_BITS + 1];
    i8 other[LETTERS][LETTERS][LETTERS];
};

struct trigram {
    char a;
    char b;
    char c;
};

typedef i8 (*op_func)(i8, i8);

struct rule {
    struct trigram i1;
    struct trigram i2;
    struct trigram o;
    op_func func;
};

u8 rules_mapping[LETTERS][LETTERS][LETTERS];

i8 and(i8 s1, i8 s2) { return s1 & s2; }
i8 or (i8 s1, i8 s2) { return s1 | s2; }
i8 xor(i8 s1, i8 s2) { return s1 ^ s2; }

char dfs(u8 i, struct rule rules[RULES_NUM], struct state *state);
void read_input(struct state *s, struct rule rules[RULES_NUM]);

void main(int argc, char const *argv[])
{
    struct rule rules[RULES_NUM];
    struct state state;
    memset(state.other, -1, sizeof(state.other));
    read_input(&state, rules);

    long total = 0;
    for (u8 i = 0; i < RULES_NUM; i++)
    {
        struct rule r = rules[i];
        if (r.o.a == 'z') {
            total |= (long)dfs(i, rules, &state) << CHAR_TO_I(o);
        }
    }
    
    printf("Total: %ld\n", total);
}

char dfs(u8 i, struct rule rules[RULES_NUM], struct state *state) {
    struct rule r = rules[i];

    i8 s1, s2;
    if (r.i1.a == 'x')
    {
        s1 = state->x[CHAR_TO_I(i1)];
        s2 = state->y[CHAR_TO_I(i2)];
    }
    else if (r.i1.a == 'y')
    {
        s1 = state->y[CHAR_TO_I(i1)];
        s2 = state->x[CHAR_TO_I(i2)];
    }
    else
    {
        s1 = state->other TRI_TO_I(i1);
        s2 = state->other TRI_TO_I(i2);
        if (s1 == -1)
        {
            s1 = dfs(rules_mapping TRI_TO_I(i1), rules, state);
        }
        if (s2 == -1)
        {
            s2 = dfs(rules_mapping TRI_TO_I(i2), rules, state);
        }
    }
    i8 s = r.func(s1, s2);
    
    if (r.o.a == 'z')
    {
        state->z[CHAR_TO_I(o)] = s;
    }
    else
    {
        state->other TRI_TO_I(o) = s;
    }

    return s;
}

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
        op_func func;
        switch (op[0])
        {
        case 'A': { func = and; break; }
        case 'O': { func = or ; break; }
        case 'X': { func = xor; break; }
        }
        struct trigram i2 = *((struct trigram*)strtok(NULL, " "));
        strtok(NULL, " "); // Skip arrow
        struct trigram o = *((struct trigram*)strtok(NULL, " "));

        rules[i] = (struct rule) { .i1=i1, .i2=i2, .func=func, .o=o };
        rules_mapping[o.a - 'a'][o.b - 'a'][o.c - 'a'] = i;
    }
    fclose(f);
}
