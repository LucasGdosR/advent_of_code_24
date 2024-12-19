#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOURCE_LEN 2904
#define TOWELS_NUM 447
#define TOWEL_MAX_LEN 9
#define PATTERNS_NUM 400
#define PATTERNS_LEN 62
#define ARENA_CAPACITY 65535
#define SENTINEL ARENA_CAPACITY
#define MAP_SIZE (ARENA_CAPACITY * 2)

typedef long i64;

struct node {
    char *key;
    i64 val;
    unsigned short next;
    unsigned short last;
};

struct hash_map {
    struct node *lists[MAP_SIZE];
    struct node arena[ARENA_CAPACITY];
    unsigned short size;
};

struct towel {
    char str[TOWEL_MAX_LEN];
    char len;
};

i64 count_arrangements(char *pattern, struct towel towels[TOWELS_NUM], struct hash_map *memo);
char startswith(char *pattern, struct towel *towel);

void add(struct hash_map *map, char *key, i64 val);
i64 get(struct hash_map *map, char *key);

void read_input(char patterns[PATTERNS_NUM][PATTERNS_LEN], struct towel towels[TOWELS_NUM]);

void main(int argc, char const *argv[])
{
    char patterns[PATTERNS_NUM][PATTERNS_LEN];
    struct towel towels[TOWELS_NUM];
    read_input(patterns, towels);
    
    struct hash_map memo;
    memo.size = 0;
    memset(memo.lists, 0, sizeof(memo.lists));
    add(&memo, "\n", 1);
    add(&memo, "\0", 1);
    
    short possible = 0;
    i64 total = 0;
    for (short i = 0; i < PATTERNS_NUM; i++)
    {
        i64 ways_to_arrange = count_arrangements((char*)patterns[i], towels, &memo);
        total += ways_to_arrange;
        possible += !!ways_to_arrange;
    }
    // munmap(t.towels); would be a waste
    printf("Patterns that can be matched: %hd\nWays to match those patterns: %ld\n", possible, total);
}

i64 count_arrangements(char *pattern, struct towel towels[TOWELS_NUM], struct hash_map *memo) {
    i64 total = get(memo, pattern);
    if (total < 0) {
        total = 0;
        for (short i = 0; i < TOWELS_NUM; i++)
        {
            struct towel *towel = &towels[i];
            if (startswith(pattern, towel)) {
                total += count_arrangements(pattern + towel->len, towels, memo);
            }
        }
        add(memo, pattern, total);
    }
    return total;
}

char startswith(char *pattern, struct towel *towel) {
    char starts = 1;
    for (char i = 0; i < towel->len; i++)
    {
        if (pattern[i] != towel->str[i])
        {
            starts = 0;
            break;
        }
    }
    return starts;
}

unsigned short hash(char *key) {
    unsigned long h = *key;
    while (*key++ != '\0')
    {
        h = h * 31 + *key;
    }
    return (unsigned short) (h % MAP_SIZE);
}

void add(struct hash_map *map, char *key, i64 val) {
    if (map->size == ARENA_CAPACITY) {
        printf("arena overflowed");
        exit(1);
    }
    
    unsigned short h = hash(key);
    struct node *head = map->lists[h];
    if (head == NULL) {
        map->arena[map->size] = (struct node) { .key=key, .val=val, .next=SENTINEL, .last=map->size };
        map->lists[h] = &map->arena[map->size++];
    } else {
        // Append to the end of the list.
        // This way, less frequently requested entries require more traversal,
        // and more frequently accessed entries require less. Optimize for the common case.
        map->arena[head->last].next = map->size;
        head->last = map->size;
        map->arena[map->size++] = (struct node) { .key=key, .val=val, .next=SENTINEL, .last=map->size };
    }
}

i64 get(struct hash_map *map, char *key) {
    unsigned short h = hash(key);
    struct node *node = map->lists[h];
    i64 val = -1;
    while (node) {
        if (!strcmp(node->key, key)) {
            val = node->val;
            break;
        }
        if (node->next == SENTINEL) {
            break;
        }
        node = &map->arena[node->next];
    }
    return val;
}

void read_input(char patterns[PATTERNS_NUM][PATTERNS_LEN], struct towel towels[TOWELS_NUM]) {
    FILE *f = fopen("19-input-target", "r");
    for (short i = 0; i < PATTERNS_NUM; i++)
        fgets(patterns[i], PATTERNS_LEN, f);
    fclose(f);

    f = fopen("19-input-source", "r");
    char buffer[SOURCE_LEN];
    fgets(buffer, SOURCE_LEN, f);
    fclose(f);

    char *token = strtok(buffer, ", ");
    for (short i = 0; i < TOWELS_NUM; i++)
    {
        strcpy(towels[i].str, token);
        towels[i].len = strlen(token);
        token = strtok(NULL, ", ");
    }
}
