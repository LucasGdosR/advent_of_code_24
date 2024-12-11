#include <math.h>
#include <stdio.h>

#define MAX_STONES 200000
#define SENTINEL_HEAD 0
#define SENTINEL_TAIL MAX_STONES
#define AFTER_SENTINELS 1

struct stone {
    __uint64_t num;
    __uint32_t prev;
    __uint32_t next;
};

struct arena {
    struct stone stones[MAX_STONES];
    __uint32_t alloc_pos;
};

void blink(__uint32_t stone_ptr, struct arena *arena) {
    __uint64_t num = arena->stones[stone_ptr].num;
    if (num == 0) {
        arena->stones[stone_ptr].num = 1;
    } else {
        __uint8_t num_of_digits = ((__uint8_t)log10(num)) + 1;
        if (num_of_digits & 1) {
            arena->stones[stone_ptr].num *= 2024;
        } else {
            __uint64_t divisor = pow(10, num_of_digits >> 1);
            struct stone left_stone;
            left_stone.num = num / divisor;
            arena->stones[stone_ptr].num = num % divisor;

            __uint32_t prev = arena->stones[stone_ptr].prev;
            arena->stones[prev].next = arena->alloc_pos;
            arena->stones[stone_ptr].prev = arena->alloc_pos;
            left_stone.prev = prev;
            left_stone.next = stone_ptr;
            arena->stones[(arena->alloc_pos)++] = left_stone;
        }
    }
}

void main(int argc, char const *argv[])
{
    __uint16_t input_stones[] = { 125, 17 };

    // Init arena with sentinels
    struct arena arena;
    arena.stones[SENTINEL_HEAD].next = SENTINEL_TAIL;
    arena.alloc_pos = AFTER_SENTINELS;

    // Init linked list with input
    {__uint32_t prev = SENTINEL_HEAD;
    for (__uint8_t i = 0; i < (sizeof(input_stones) / sizeof(__uint16_t)); i++) {
        arena.stones[arena.alloc_pos] = (struct stone) { .num = input_stones[i], .prev = prev, .next = SENTINEL_TAIL };
        arena.stones[prev].next = arena.alloc_pos;
        prev = arena.alloc_pos++;
    }}
    
    // Simulation
    for (__uint8_t i = 0; i < 25; i++) {
        __uint32_t curr = arena.stones[SENTINEL_HEAD].next;
        while (curr != SENTINEL_TAIL) {
            blink(curr, &arena);
            curr = arena.stones[curr].next;
        }
    }

    // Counting stones
    __uint32_t curr = arena.stones[SENTINEL_HEAD].next, stone_count = 0;
    while (curr != SENTINEL_TAIL) {
        curr = arena.stones[curr].next;
        stone_count++;
    }
    printf("This many stones: %d\n", stone_count);
}
