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
#define FALLEN_BYTES 1024

struct p {
    __uint8_t i;
    __uint8_t j;
};

struct path {
    struct p p;
    __uint16_t steps;
};

struct queue {
    struct path ps[QUEUE_SIZE];
    __uint16_t start;
    __uint16_t end;
};

void enqueue(struct queue *q, struct path p) {
    q->ps[q->end++] = p;
    q->end &= QUEUE_MASK;
}

struct path dequeue(struct queue *q) {
    struct path p = q->ps[q->start++];
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

void place_bytes(char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS], struct p falling_bytes[INPUT_LENGTH]) {
    for (__uint16_t i = 0; i < FALLEN_BYTES; i++)
    {
        struct p p = falling_bytes[i];
        grid[p.i + SENTINEL][p.j + SENTINEL] = 0;
    }
}

__uint16_t bfs(char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS]) {
    struct queue q;
    q.start = 0, q.end = 1, q.ps[0] = (struct path) { .p={ .i=SENTINEL, .j=SENTINEL }, .steps=0 };
    while (q.start != q.end)
    {
        struct path path = dequeue(&q);
        if (path.p.i == EXIT_PLUS_SENTINEL && path.p.j == EXIT_PLUS_SENTINEL) {
            return path.steps;
        }
        #define ADD_NEIGHBOR(di, dj) if (grid[path.p.i + (di)][path.p.j + (dj)]) { \
            grid[path.p.i + (di)][path.p.j + (dj)] = 0; \
            enqueue(&q, (struct path) { \
                .p = { .i = path.p.i + (di), .j = path.p.j + (dj)}, \
                .steps=path.steps + 1 \
                }); \
        }
        ADD_NEIGHBOR(-1,  0)
        ADD_NEIGHBOR( 1,  0)
        ADD_NEIGHBOR( 0, -1)
        ADD_NEIGHBOR( 0,  1)
    }
}

void main(int argc, char const *argv[])
{
    struct p falling_bytes[INPUT_LENGTH];
    read_input(falling_bytes);
    char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS] = {};
    init_grid(grid);
    place_bytes(grid, falling_bytes);
    __uint16_t steps = bfs(grid);
    printf("Steps from start to exit with 1024 fallen bytes: %hd\n", steps);
}
