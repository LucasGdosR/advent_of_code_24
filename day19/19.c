#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define INPUT_1_SIZE 447
#define INPUT_2_NUM 400
#define INPUT_2_LEN 62
#define ARENA_CAPACITY 3000
#define MAP_SIZE (ARENA_CAPACITY * 2)

typedef long i64;

struct available_towels {
    char *towels;
    short size;
};

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

i64 count_arrangements(char *pattern, struct hash_map *memo);

void add(struct hash_map *map, char *key, i64 val);
i64 get(struct hash_map *map, char *key);

struct available_towels read_input(char patterns[INPUT_2_NUM][INPUT_2_LEN]);

void main(int argc, char const *argv[])
{
    char patterns[INPUT_2_NUM][INPUT_2_LEN];
    struct available_towels t = read_input(patterns);

    struct hash_map memo;
    memo.size = 0;
    memset(memo.lists, 0, sizeof(memo.lists));

    add(&memo, "", 1);

    short possible = 0;
    i64 total = 0;
    for (short i = 0; i < INPUT_2_NUM; i++)
    {
        i64 ways_to_arrange = count_arrangements((char*)patterns[i], t, &memo);
        total += ways_to_arrange;
        possible += !!ways_to_arrange;
    }
    // munmap(t.towels); would be a waste
    printf("Patterns that can be matched: %hd\n Ways to match those patterns: %ld\n", possible, total);
}

i64 count_arrangements(char *pattern, struct available_towels t, struct hash_map *memo) {
    i64 total = get(memo, pattern);
    if (total < 0) {
        total = // sum(count_arrangements(pattern[len(towel):]) for towel in available_towels if pattern.startswith(towel));
        add(memo, pattern, total);
    }
    return total;
}

unsigned short hash(char *key) {
    i64 h = *key;
    while (*key++ != '\0')
    {
        h = h * 31 + *key;
    }
    return (unsigned short) (h % MAP_SIZE);
}

void add(struct hash_map *map, char *key, i64 val) {
    if (map->size == ARENA_CAPACITY) {
        printf("arena overflowed");
        exit(1); // include sys
    }

    unsigned short h = hash(key);
    struct node *head = map->lists[h];
    if (head == NULL) {
        map->arena[map->size] = (struct node) { .key=key, .val=val, .next=NULL, .last=map->size };
        map->lists[h] = &map->arena[map->size++];
    } else {
        // Append to the end of the list.
        // This way, less frequently requested entries require more traversal,
        // and more frequently accessed entries require less. Optimize for the common case.
        map->arena[head->last].next = map->size;
        head->last = map->size;
        map->arena[map->size++] = (struct node) { .key=key, .val=val, .next=NULL, .last=map->size };
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
        // advance list
    }
    return val;
}

struct available_towels read_input(char patterns[INPUT_2_NUM][INPUT_2_LEN]) {
    FILE *f = fopen("16-input-patterns", "r");
    for (short i = 0; i < INPUT_2_LEN; i++)
        fgets(patterns[i], INPUT_2_LEN, fptr);
    fclose(fptr);

    struct available_towels t;
    int fd = open("16-input-available", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    t.size = sb.st_size;
    t.towels = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    return t;
}
