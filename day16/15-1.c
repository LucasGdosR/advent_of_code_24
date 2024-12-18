#include <stdio.h>

#define MAZE_SIZE 1 // TODO
#define HEAP_SIZE 1 // TODO
#define SET_SIZE 1 // TODO

typedef unsigned char  u8 ;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

struct p {
    u8 i;
    u8 j;
};

enum Facing                            {  EAST ,  SOUTH,  WEST  ,  NORTH  };
const enum Facing ROTATE_CLOCKWISE[] = {  SOUTH,  WEST ,  NORTH ,  EAST   };
const enum Facing ROTATE_COUNTER_C[] = {  NORTH,  EAST ,  SOUTH ,  WEST   };
const struct p          DIRECTIONS[] = { {.j=1}, {.i=1}, {.j=-1}, {.i=-1} };

struct state {
    u16 score;
    struct p p;
    enum Facing f;
};

struct heap {
    struct state states[HEAP_SIZE];
    u16 size;
};

struct set {
    // TODO
};

void main(int argc, char const *argv[])
{
    char maze[MAZE_SIZE][MAZE_SIZE];
    read_input(maze);
    struct p start = find_start(maze);
    struct heap h;
    h.size = 1, h.states[0] = (struct state) { .score=0, .p=start, .f=EAST };
    u16 score = find_shortest_path(&h);
    printf("Score: %hd\n", score);
}

u16 score find_shortest_path(struct heap *h) {
    struct set visited;
    while (h->size)
    {
        /* code */
    }
}

struct p find_start(char maze[MAZE_SIZE][MAZE_SIZE + 2]) {
    for (u8 i = 0; i < MAZE_SIZE; i++)
        for (u8 j = 0; j < MAZE_SIZE; j++)
            if (maze[i][j] == 'S')
                return (struct p) { .i=i, .j=j };
}

void read_input(char maze[MAZE_SIZE][MAZE_SIZE + 2]) {
    FILE *f = fopen("16-input", "r");
    for (u8 i = 0; i < MAZE_SIZE; i++)
    {
        fgets(maze[i], MAZE_SIZE + 2, f);
    }
    fclose(f);
}
