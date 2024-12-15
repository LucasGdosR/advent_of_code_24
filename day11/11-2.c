#include <math.h>
#include <stdio.h>
#include <string.h>

#define HASHTABLE_FINAL_SIZE 133143
#define HASHTABLE_CAPACITY (2 * HASHTABLE_FINAL_SIZE)
#define SENTINEL 1
#define HASHTABLE_MOD(hash) ((hash % (HASHTABLE_CAPACITY - SENTINEL)) + SENTINEL)
#define GET_NUM_OF_DIGITS(num) (((__uint8_t)log10(num)) + 1)
#define IS_ODD(num_of_digits) (num_of_digits & 1)

// 32-byte aligned, preventing false sharing
// *next could be a 32-bit integer representing an index,
// but that would lead to false sharing,
// possibly worsening cache performance
struct node {
    __uint64_t num;
    __uint64_t stone_count;
    __uint8_t n_blinks;
    struct node *next;
};

struct hash_table {
    struct node *table[HASHTABLE_CAPACITY];
    struct node arena[HASHTABLE_FINAL_SIZE];
    __uint32_t alloc_pos;
};

__uint64_t blink_n_times(__uint64_t num, __uint8_t n_blinks, struct hash_table *cache);
void put(struct hash_table *cache, __uint64_t num, __uint8_t n_blinks, __uint64_t stone_count);
__uint64_t get(struct hash_table *cache, __uint64_t num, __uint8_t n_blinks);

void main(int argc, char const *argv[])
{
    // Input
    __uint64_t input_stones[] = { 125, 17 };

    // Init hash table
    struct hash_table cache;
    memset(cache.table, 0, sizeof(cache.table));
    cache.alloc_pos = 0;

    // Count
    __uint64_t stone_count = 0;
    for (__uint8_t i = 0; i < (sizeof(input_stones) / sizeof(__uint64_t)); i++)
    {
        stone_count += blink_n_times(input_stones[i], 75, &cache);
    }

    printf("This many stones: %ld\n", stone_count);
}

__uint64_t blink_n_times(__uint64_t num, __uint8_t n_blinks, struct hash_table *cache) {
    // Look in the cache
    __uint64_t stone_count = get(cache, num, n_blinks);
    if (!stone_count)
    {
        // Base case
        if (n_blinks == 1)
        {
            stone_count = (num == 0) || IS_ODD(GET_NUM_OF_DIGITS(num)) ? 1 : 2;
        } else {
            // Recurse
            if (num == 0)
            {
                stone_count = blink_n_times(1, n_blinks - 1, cache);
            } else {
                __uint8_t num_of_digits = GET_NUM_OF_DIGITS(num);
                if (IS_ODD(num_of_digits))
                {
                    stone_count = blink_n_times(2024 * num, n_blinks - 1, cache);
                } else {
                    __uint64_t divisor = round(pow(10, num_of_digits >> 1));
                    __uint64_t num1 = num / divisor;
                    __uint64_t num2 = num % divisor;
                    stone_count = blink_n_times(num1, n_blinks - 1, cache)
                                + blink_n_times(num2, n_blinks - 1, cache);
                }
            }
            put(cache, num, n_blinks, stone_count);
        }
    }
    return stone_count;
}

__uint64_t hash(__uint64_t num, __uint8_t n_blinks) {
    const __uint64_t PRIME1 = 1699;
    const __uint64_t PRIME2 = 7919;
    const __uint64_t PRIME3 = 0xff51afd7ed558ccd;
    
    __uint64_t h = (num * PRIME1) ^ ((__uint64_t)n_blinks * PRIME2);
    h ^= h >> 33;
    h *= PRIME3;
    h ^= h >> 33;
    
    return h;
}

void put(struct hash_table *cache, __uint64_t num, __uint8_t n_blinks, __uint64_t stone_count) {
    __uint64_t h = HASHTABLE_MOD(hash(num, n_blinks));
    cache->arena[cache->alloc_pos] = (struct node) {
        .num=num,
        .n_blinks=n_blinks,
        .stone_count=stone_count,
        .next=cache->table[h]
        };
    cache->table[h] = &cache->arena[cache->alloc_pos++];
}

__uint64_t get(struct hash_table *cache, __uint64_t num, __uint8_t n_blinks) {
    __uint64_t h = HASHTABLE_MOD(hash(num, n_blinks));
    struct node *curr = cache->table[h];
    while (curr) {
        if (curr->num == num && curr->n_blinks == n_blinks) {
            return curr->stone_count;
        }
        curr = curr->next;
    }
    return 0;
}
