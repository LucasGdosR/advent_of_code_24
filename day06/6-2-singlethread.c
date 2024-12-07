#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 130
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2
#define OBSTACLE '#'
#define NOT_VISITED '.'
#define DIRECTIONS 4
#define HASHTABLE_SIZE 10007 // Prime number
#define REGULAR_PATH_LENGTH 5444

enum Facing {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct point {
    uint8_t x;
    uint8_t y;
};

struct guard {
    enum Facing facing;
    struct point p;
};

struct node {
    struct guard state;
    struct node *next;
};

struct set {
    struct node *states[HASHTABLE_SIZE];
    struct node arena[HASHTABLE_SIZE];
    uint16_t arena_alloc_pos;
};

struct node NIL_NODE;

struct guard find_guard();
bool within_bounds(struct point p);
struct point move_to_exit_and_record_path(struct guard *guard, uint16_t *visited);
bool test_loop(struct set *states, struct guard *g, uint16_t *loops_found);

void set_init(struct set *set);
bool set_add(struct set *s, struct guard *g);
bool set_has(struct set *s, struct guard *g, uint32_t i);
uint32_t set_hash(struct guard *g);

void read_input();
void add_sentinels();
void init_nil_node();

char input[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

void main(int argc, char const *argv[]) {
    read_input();
    add_sentinels();
    init_nil_node();
    struct guard initial_guard = find_guard();
    struct guard guard = initial_guard;

    // 5444 is the answer from the first part
    // for the input I personally received
    struct point regular_path[REGULAR_PATH_LENGTH];
        
    // Part 1 is still solved for getting the path the guard normally takes
    // to reduce the possibilities of obstacle placements in part 2
    uint16_t visited = 0;
    while (within_bounds(guard.p)) {
        struct point p = move_to_exit_and_record_path(&guard, &visited);
        if (p.x) {
            regular_path[visited] = p;
        }
    }
    printf("Unique tiles visited: %d\n", ++visited);

    // Part 2
    // Storing the path from part 1 allows part 2 to test only relevant tiles for obstacles
    clock_t begin = clock();
    struct set states;
    uint16_t loops_found = 0;
    for (uint16_t i = 0; i < REGULAR_PATH_LENGTH; i++) {
        set_init(&states);
        struct guard g = initial_guard;

        struct point obstacle = regular_path[i];
        input[obstacle.y][obstacle.x] = OBSTACLE;
        while (test_loop(&states, &g, &loops_found));
        input[obstacle.y][obstacle.x] = NOT_VISITED;
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Places leading to loops: %d\nTime spent computing: %f seconds\n", loops_found, time_spent);
}

struct guard find_guard() {
    for (uint8_t i = 1; i <= GRID_SIZE; i++) {
        for (uint8_t j = 1; j < GRID_SIZE; j++) {
            if (input[i][j] == '^')
                return (struct guard) { .facing = UP, .p = (struct point) { .x = j, .y = i } };
        }
    }
}

bool within_bounds(struct point p) {
    return (p.x != 0) && (p.x != GRID_SIZE_PLUS_SENTINELS - 1)
        && (p.y != 0) && (p.y != GRID_SIZE_PLUS_SENTINELS - 1);
}

struct point move_to_exit_and_record_path(struct guard *guard, uint16_t *visited) {
    struct point destination = guard->p;
    enum Facing facing = guard->facing;
    destination.x += (facing == RIGHT) - (facing == LEFT);
    destination.y += (facing ==  DOWN) - (facing == UP);
    switch (input[destination.y][destination.x])
    {
    case NOT_VISITED:
        input[destination.y][destination.x] = 'X';
        (*visited)++;
        guard->p = destination;
        return destination;
    case OBSTACLE:
        guard->facing = (guard->facing + 1) % DIRECTIONS;
        return (struct point) { .x = 0, .y = 0 };
    default:
        guard->p = destination;
        return (struct point) { .x = 0, .y = 0 };
    }
}

bool test_loop(struct set *states, struct guard *g, uint16_t *loops_found) {
    struct point dst = g->p;
    enum Facing f = g->facing;
    dst.x += (f == RIGHT) - (f == LEFT);
    dst.y += (f ==  DOWN) - (f == UP);

    if (!within_bounds(dst)) {
        return false;
    }

    if (input[dst.y][dst.x] == OBSTACLE) {
        g->facing = (g->facing + 1) % DIRECTIONS;
    } else {
        g->p = dst;
    }

    if (set_add(states, g)) {
        return true;
    }
    (*loops_found)++;
    return false;
}

void set_init(struct set *set) {
    struct node **states = set->states;
    for (uint16_t i = 0; i < HASHTABLE_SIZE; i++) {
        states[i] = &NIL_NODE;
    }
    set->arena_alloc_pos = 0;
}

bool set_add(struct set *s, struct guard *g) {
    int i = set_hash(g);
    if (set_has(s, g, i)) {
        return false;
    }
    s->arena[s->arena_alloc_pos] = (struct node) {
        .state = *g,
        .next = s->states[i]
        };
    s->states[i] = &s->arena[s->arena_alloc_pos++];
    return true;
}

bool set_has(struct set *s, struct guard *g, uint32_t i) {
    struct guard guard = *g;
    struct node *curr = s->states[i];
    while (curr != &NIL_NODE) {
        struct guard state = curr->state;
        curr = curr->next;
        if (state.facing == guard.facing
         && state.p.x == guard.p.x
         && state.p.y == guard.p.y) {
            return true;
        }
    }
    return false;
}

uint32_t set_hash(struct guard *g) {
    struct guard state = *g;
    uint32_t hash = state.facing;
    hash = hash * 131 + state.p.x;
    hash = hash * 131 + state.p.y;
    return hash % HASHTABLE_SIZE;
}

void read_input() {
    FILE *fptr = fopen("6-input", "r");
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        fgets(input[i + 1] + 1, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}

void add_sentinels() {
    for (uint8_t i = 0; i < GRID_SIZE_PLUS_SENTINELS; i++) {
        input[0][i] = 0;
        input[GRID_SIZE_PLUS_SENTINELS - 1][i] = 0;
        input[i][0] = 0;
        input[i][GRID_SIZE_PLUS_SENTINELS - 1] = 0;
    } 
}

void init_nil_node() {
    NIL_NODE = (struct node) {
        .state = (struct guard) {
            .facing = UP,
            .p = (struct point) {
                .x = 0,
                .y = 0
                }
            },
        .next = &NIL_NODE
        };
}
