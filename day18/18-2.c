#include <stdio.h>

#define GRID_SIZE 71
#define SENTINEL 1
#define GRID_SIZE_PLUS_SENTINELS (GRID_SIZE + 2)
#define EXIT (GRID_SIZE - 1)
#define EXIT_PLUS_SENTINEL (EXIT + SENTINEL)
#define QUEUE_POW_2_CONSTANT 8
#define QUEUE_SIZE (1 << QUEUE_POW_2_CONSTANT)
#define QUEUE_MASK (QUEUE_SIZE - 1)
#define INPUT_LENGTH 3450
#define LINE_LENGTH 7

struct p {
    __uint8_t i;
    __uint8_t j;
};

struct queue {
    struct p ps[QUEUE_SIZE];
    __uint16_t start;
    __uint16_t end;
};

void enqueue(struct queue *q, struct p p) {
    q->ps[q->end++] = p;
    q->end &= QUEUE_MASK;
}

struct p dequeue(struct queue *q) {
    struct p p = q->ps[q->start++];
    q->start &= QUEUE_MASK;
    return p;
}

void read_input(struct p falling_bytes[INPUT_LENGTH]) {
    FILE *f = fopen("18-input", "r");
    char buffer[LINE_LENGTH];
    for (__uint16_t i = 0; i < INPUT_LENGTH; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        sscanf(buffer, "%hhd,%hhd", &falling_bytes[i].i, &falling_bytes[i].j);
    }
}

void init_grid(char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS]) {
    for (__uint8_t i = SENTINEL; i < GRID_SIZE_PLUS_SENTINELS - SENTINEL; i++)
    {
        for (__uint8_t j = SENTINEL; j < GRID_SIZE_PLUS_SENTINELS - SENTINEL; j++)
        {
            grid[i][j] = 1;
        }
    }
}

void place_bytes(char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS], __uint16_t fallen_bytes, struct p falling_bytes[INPUT_LENGTH]) {
    for (__uint16_t i = 0; i < fallen_bytes; i++)
    {
        struct p p = falling_bytes[i];
        grid[p.i + SENTINEL][p.j + SENTINEL] = 0;
    }
}

__uint16_t bfs(char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS]) {
    struct queue q;
    q.start = 0, q.end = 1, q.ps[0] = (struct p) { .i=SENTINEL, .j=SENTINEL };
    while (q.start != q.end)
    {
        struct p p = dequeue(&q);
        if (p.i == EXIT_PLUS_SENTINEL && p.j == EXIT_PLUS_SENTINEL) {
            return 1;
        }
        #define ADD_NEIGHBOR(di, dj) if (grid[p.i + (di)][p.j + (dj)]) { \
            grid[p.i + (di)][p.j + (dj)] = 0; \
            enqueue(&q, (struct p) { .i = p.i + (di), .j = p.j + (dj) }); \
        }
        ADD_NEIGHBOR(-1,  0)
        ADD_NEIGHBOR( 1,  0)
        ADD_NEIGHBOR( 0, -1)
        ADD_NEIGHBOR( 0,  1)
    }
    return 0;
}

__uint16_t binary_search(
    __uint16_t lower_bound, __uint16_t upper_bound,
    char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS],
    struct p falling_bytes[INPUT_LENGTH])
{
    while (lower_bound < upper_bound)
    {
        __uint16_t m = (lower_bound + upper_bound) / 2;
        init_grid(grid);
        place_bytes(grid, m, falling_bytes);
        if (bfs(grid)) {
            lower_bound = m + 1;
        } else {
            upper_bound = m;
        }
    }
    return lower_bound;
}

void main(int argc, char const *argv[])
{
    struct p falling_bytes[INPUT_LENGTH];
    read_input(falling_bytes);
    char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS] = {};

    struct p byte_of_doom = falling_bytes[binary_search(1024, INPUT_LENGTH, grid, falling_bytes) - 1];

    printf("Coordinates of byte that prevents exit: %hhd, %hhd\n", byte_of_doom.i, byte_of_doom.j);
}
