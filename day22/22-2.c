#include <stdio.h>

#define INPUT_SIZE 1506
#define BUFFER_SIZE 10
#define SEQUENCE_LENGTH 4
#define SEQ_VAL_RANGE 19
#define MAX_PRICE_SWING 9
#define POSSIBLE_SEQUENCES (19 * 19 * 19 * 19)

struct buffered_q {
    signed char changes[SEQUENCE_LENGTH];
    char curr;
};

int init_price_changes(struct buffered_q *price_changes, int seed);
void overwrite(struct buffered_q *price_changes, signed char price_change);
int hash(struct buffered_q price_changes);
int random(int seed);
void read_input(int seeds[INPUT_SIZE]);

void main(int argc, char const *argv[])
{
    int seeds[INPUT_SIZE];
    read_input(seeds);

    short profits[POSSIBLE_SEQUENCES] = {};

    for (short i = 0; i < INPUT_SIZE; i++) {
        int seed = seeds[i];

        char seen_sequences[POSSIBLE_SEQUENCES] = {};
        struct buffered_q price_changes;
        seed = init_price_changes(&price_changes, seed);
        
        int h = hash(price_changes);
        profits[h] += (seed % 10) * (!seen_sequences[h]);
        seen_sequences[h] = 1;

        for (size_t i = 4; i < 2000; i++)
        {
            char prev_price = seed % 10;
            seed = random(seed);
            char curr_price = seed % 10;
            overwrite(&price_changes, curr_price - prev_price);
            int h = hash(price_changes);
            profits[h] += curr_price * (!seen_sequences[h]);
            seen_sequences[h] = 1;
        }
    }
    
    short max_profit = 0;
    for (int i = 0; i < POSSIBLE_SEQUENCES; i++)
    {
        max_profit = max_profit >= profits[i] ? max_profit : profits[i];
    }
    printf("Maximum possible profit: %hd\n", max_profit);
}

int init_price_changes(struct buffered_q *price_changes, int seed) {
    price_changes->curr = 0;
    char prev_price, curr_price;
    #define _PUT_IN_Q_(_i_) prev_price = seed % 10; \
    seed = random(seed); \
    curr_price = seed % 10; \
    price_changes->changes[(_i_)] = curr_price - prev_price;
    _PUT_IN_Q_(0)
    _PUT_IN_Q_(1)
    _PUT_IN_Q_(2)
    _PUT_IN_Q_(3)
    return seed;
}

void overwrite(struct buffered_q *price_changes, signed char price_change) {
    price_changes->changes[price_changes->curr++] = price_change;
    price_changes->curr %= SEQUENCE_LENGTH;
}

int hash(struct buffered_q price_changes) {
    int h = price_changes.changes[price_changes.curr] + MAX_PRICE_SWING;
    for (char i = price_changes.curr + 1; i < SEQUENCE_LENGTH; i++) {
        h = h * 19 + price_changes.changes[i] + MAX_PRICE_SWING;
    }
    for (char i = 0; i < price_changes.curr; i++) {
        h = h * 19 + price_changes.changes[i] + MAX_PRICE_SWING;
    }
    return h;
}

int random(int seed) {
    seed = (seed ^ (seed << 6)) & 0xFFFFFF;
    seed = (seed ^ (seed >> 5)) & 0xFFFFFF;
    return (seed ^ (seed << 11)) & 0xFFFFFF;
}

void read_input(int seeds[INPUT_SIZE]) {
    FILE *f = fopen("22-input", "r");
    char buffer[BUFFER_SIZE];
    for (size_t i = 0; i < INPUT_SIZE; i++)
    {
        fgets(buffer, BUFFER_SIZE, f);
        sscanf(buffer, "%d", &seeds[i]);
    }
    fclose(f);
}
