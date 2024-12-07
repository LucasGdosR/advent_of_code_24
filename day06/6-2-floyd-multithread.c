#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 130
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 4
#define OBSTACLE '#'
#define NOT_VISITED '.'
#define DIRECTIONS 4
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

struct guard_in_new_point {
    struct guard g;
    bool new_point;
};

struct job_arg {
    struct point *regular_path;
    struct guard initial_guard;
};

uint16_t visited = 0;
_Atomic uint16_t paths_shared_index = 0;

struct guard find_guard();
bool within_bounds(struct point p);
struct guard_in_new_point move_and_record_path(struct guard_in_new_point g);

bool floyd_cycle_detection(struct guard starting_state, struct point obstacle);
struct guard move(struct guard g, struct point obstacle);

void read_input();
void add_sentinels();
void *job(void *arg);

char input[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

void main(int argc, char const *argv[]) {
    read_input();
    add_sentinels();
    struct guard initial_guard = find_guard();
    struct guard_in_new_point g = (struct guard_in_new_point) { .g = initial_guard, .new_point = true };

    // 5444 is the answer from the first part
    // for the input I personally received
    struct point regular_path[REGULAR_PATH_LENGTH];
        
    // Part 1 is still solved for getting the path the guard normally takes
    // to reduce the possibilities of obstacle placements in part 2
    while (within_bounds(g.g.p)) {
        g = move_and_record_path(g);
        if (g.new_point) {
            regular_path[visited] = g.g.p;
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

bool floyd_cycle_detection(struct guard starting_state, struct point obstacle) {
    struct guard tortoise = starting_state;
    struct guard hare = starting_state;
    for(;;) {
        tortoise = move(tortoise, obstacle);
        
        hare = move(move(hare, obstacle), obstacle);
        if (!within_bounds(hare.p)) {
            return false;
        }
        
        if (tortoise.facing == hare.facing &&
            tortoise.p.x == hare.p.x &&
            tortoise.p.y == hare.p.y) {
            return true;
        }
    }
}

struct guard move(struct guard g, struct point obstacle) {
    struct point dst = g.p;
    dst.x += (g.facing == RIGHT) - (g.facing == LEFT);
    dst.y += (g.facing ==  DOWN) - (g.facing == UP);

    if (input[dst.y][dst.x] == OBSTACLE
     || (dst.y == obstacle.y && dst.x == obstacle.x)) {
        g.facing = (g.facing + 1) % DIRECTIONS;
    } else {
        g.p = dst;
    }
    return g;
}

void *job(void *arg) {
    struct job_arg ja = *(struct job_arg*)arg;
    uint16_t *loops_found = malloc(sizeof(uint16_t));
    *loops_found = 0;
    for (;;) {
        uint16_t curr_index = paths_shared_index++;
        if (curr_index >= REGULAR_PATH_LENGTH) {
            pthread_exit((void *)loops_found);
        }
        *loops_found += floyd_cycle_detection(ja.initial_guard, ja.regular_path[curr_index]);
    }
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
    return (p.x > 1) && (p.x < GRID_SIZE_PLUS_SENTINELS - 2)
        && (p.y > 1) && (p.y < GRID_SIZE_PLUS_SENTINELS - 2);
}

struct guard_in_new_point move_and_record_path(struct guard_in_new_point g) {
    struct point dst = g.g.p;
    dst.x += (g.g.facing == RIGHT) - (g.g.facing == LEFT);
    dst.y += (g.g.facing ==  DOWN) - (g.g.facing == UP);
    switch (input[dst.y][dst.x])
    {
    case OBSTACLE:
        g.g.facing = (g.g.facing + 1) % DIRECTIONS;
        g.new_point = false;
        break;
    case NOT_VISITED:
        input[dst.y][dst.x] = 'X';
        visited++;
        g.g.p = dst;
        g.new_point = true;
        break;
    default:
        g.g.p = dst;
        g.new_point = false;
        break;
    }
    return g;
}

void read_input() {
    FILE *fptr = fopen("6-input", "r");
    for (uint8_t i = 0; i < GRID_SIZE; i++) {
        fgets(input[i + 2] + 2, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}

void add_sentinels() {
    for (uint8_t i = 0; i < GRID_SIZE_PLUS_SENTINELS; i++) {
        input[0][i] = 0;
        input[1][i] = 0;
        input[GRID_SIZE_PLUS_SENTINELS - 2][i] = 0;
        input[GRID_SIZE_PLUS_SENTINELS - 1][i] = 0;
        input[i][0] = 0;
        input[i][1] = 0;
        input[i][GRID_SIZE_PLUS_SENTINELS - 2] = 0;
        input[i][GRID_SIZE_PLUS_SENTINELS - 1] = 0;
    } 
}
