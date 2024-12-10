#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GRID_SIZE 130
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2
#define OBSTACLE '#'
#define NOT_VISITED '.'
#define DIRECTIONS 4
#define HASHTABLE_SIZE 10007 // Prime number
#define REGULAR_PATH_LENGTH 5444
#define THREADS 4

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

struct job_arg {
    struct point *regular_path;
    struct guard initial_guard;
};

uint16_t visited;
_Atomic uint16_t paths_shared_index;

struct guard find_guard();
bool within_bounds(struct point p);
struct point move_to_exit_and_record_path(struct guard *guard);
bool test_loop(struct set *states, struct guard *g, struct point obstacle, uint16_t *loops_found);

void set_init(struct set *set);
bool set_add(struct set *s, struct guard g);
bool set_has(struct set *s, struct guard g, uint32_t i);
uint32_t set_hash(struct guard g);

void read_input();
void *job(void *arg);

char input[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

void main(int argc, char const *argv[]) {
    read_input();
    struct guard initial_guard = find_guard();
    struct guard guard = initial_guard;

    // 5444 is the answer from the first part
    // for the input I personally received
    struct point regular_path[REGULAR_PATH_LENGTH];
        
    // Part 1 is still solved for getting the path the guard normally takes
    // to reduce the possibilities of obstacle placements in part 2
    while (within_bounds(guard.p)) {
        struct point p = move_to_exit_and_record_path(&guard);
        if (p.x) {
            regular_path[visited] = p;
        }
    }
    printf("Unique tiles visited: %d\n", ++visited);

    // Part 2
    // Storing the path from part 1 allows part 2 to test only relevant tiles for obstacles
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    
    struct job_arg arg = { .initial_guard = initial_guard, .regular_path = regular_path };
    pthread_t tid[THREADS];
    uint16_t *result;
    uint16_t loops_found = 0;
    for (uint8_t i = 0; i < THREADS; i++) {
        pthread_create(&tid[i], NULL, job, (void *)&arg);
    }
    for (uint8_t i = 0; i < THREADS; i++) {
        pthread_join(tid[i], (void **)&result);
        loops_found += *result;
        free(result);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;
    printf("Places leading to loops: %d\nTime spent computing: %f seconds\n", loops_found, elapsed);
}

void *job(void *arg) {
    struct job_arg ja = *(struct job_arg*)arg;
    struct set states;
    uint16_t *loops_found = malloc(sizeof(uint16_t));
    for (;;) {
        uint16_t curr_index = paths_shared_index++;
        if (curr_index >= REGULAR_PATH_LENGTH) {
            pthread_exit((void *)loops_found);
        }

        set_init(&states);
        struct guard g = ja.initial_guard;
        while (test_loop(&states, &g, ja.regular_path[curr_index], loops_found));
    }
}

struct guard find_guard() {
    for (uint8_t i = 1; i <= GRID_SIZE; i++) {
        for (uint8_t j = 1; j <= GRID_SIZE; j++) {
            if (input[i][j] == '^')
                return (struct guard) { .p = (struct point) { .x = j, .y = i } };
        }
    }
}

bool within_bounds(struct point p) {
    return (p.x != 0) && (p.x != GRID_SIZE_PLUS_SENTINELS - 1)
        && (p.y != 0) && (p.y != GRID_SIZE_PLUS_SENTINELS - 1);
}

struct point move_to_exit_and_record_path(struct guard *guard) {
    struct point destination = guard->p;
    enum Facing facing = guard->facing;
    destination.x += (facing == RIGHT) - (facing == LEFT);
    destination.y += (facing ==  DOWN) - (facing == UP);
    switch (input[destination.y][destination.x])
    {
    case NOT_VISITED:
        input[destination.y][destination.x] = 'X';
        visited++;
        guard->p = destination;
        return destination;
    case OBSTACLE:
        guard->facing = (guard->facing + 1) % DIRECTIONS;
        return (struct point) {};
    default:
        guard->p = destination;
        return (struct point) {};
    }
}

bool test_loop(struct set *states, struct guard *g, struct point obstacle, uint16_t *loops_found) {
    struct point dst = g->p;
    enum Facing f = g->facing;
    dst.x += (f == RIGHT) - (f == LEFT);
    dst.y += (f ==  DOWN) - (f == UP);

    if (!within_bounds(dst)) {
        return false;
    }

    if (input[dst.y][dst.x] == OBSTACLE
     || (dst.y == obstacle.y && dst.x == obstacle.x)) {
        g->facing = (g->facing + 1) % DIRECTIONS;
    } else {
        g->p = dst;
    }

    if (set_add(states, *g)) {
        return true;
    }
    (*loops_found)++;
    return false;
}

void set_init(struct set *set) {
    memset(set->states, 0, sizeof(struct state*) * HASHTABLE_SIZE);
    set->arena_alloc_pos = 0;
}

bool set_add(struct set *s, struct guard g) {
    int i = set_hash(g);
    if (set_has(s, g, i)) {
        return false;
    }
    s->arena[s->arena_alloc_pos] = (struct node) {
        .state = g,
        .next = s->states[i]
        };
    s->states[i] = &s->arena[s->arena_alloc_pos++];
    return true;
}

bool set_has(struct set *s, struct guard g, uint32_t i) {
    struct node *curr = s->states[i];
    while (curr) {
        struct guard state = curr->state;
        curr = curr->next;
        // Testing two out of three is sufficient
        // but testing all three is remarkably faster
        if (state.facing == g.facing
         && state.p.x == g.p.x
         && state.p.y == g.p.y) {
            return true;
        }
    }
    return false;
}

uint32_t set_hash(struct guard g) {
    return (131*131*g.facing + 131*g.p.x + g.p.y) % HASHTABLE_SIZE;
}

void read_input() {
    FILE *fptr = fopen("6-input", "r");
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        fgets(input[i + 1] + 1, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}
