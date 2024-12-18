#include <stdio.h>

#define MAZE_SIZE 141
#define HEAP_SIZE 1024
#define HEAP_START 1
#define SET_SIZE (((MAZE_SIZE - 1) << 8) | (MAZE_SIZE - 1))

typedef signed   char  i8 ;
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
    struct p p;
    enum Facing f;
};

struct path {
    u32 score;
    struct state s;
};

struct heap {
    struct path paths[HEAP_SIZE];
    u16 size;
};

u32 find_shortest_path(char maze[MAZE_SIZE][MAZE_SIZE + 2], struct heap *h);
struct p find_start(char maze[MAZE_SIZE][MAZE_SIZE + 2]);

u8 set_add(u8 set[SET_SIZE], struct state state);

struct path heap_pop(struct heap *h);
void heap_insert(struct heap *h, struct path p);

void read_input(char maze[MAZE_SIZE][MAZE_SIZE + 2]);

void main(int argc, char const *argv[])
{
    char maze[MAZE_SIZE][MAZE_SIZE + 2];
    read_input(maze);
    struct p start = find_start(maze);
    struct heap h;
    h.size = 1, h.paths[HEAP_START] = (struct path) { .score=0, .s={ .p=start, .f=EAST } };
    u32 score = find_shortest_path(maze, &h);
    printf("Score: %d\n", score);
}

u32 find_shortest_path(char maze[MAZE_SIZE][MAZE_SIZE + 2], struct heap *h) {
    u8 visited[SET_SIZE] = {};
    set_add(visited, h->paths[HEAP_START].s);

    while (h->size)
    {
        struct path curr = heap_pop(h);
        
        // Trying to move forward
        struct p d = DIRECTIONS[curr.s.f];
        u8 next_i = curr.s.p.i + d.i, next_j = curr.s.p.j + d.j;
        char tile = maze[next_i][next_j];
        
        if (tile == 'E') {
            return curr.score + 1;
        } else if (tile == '.') {
            struct state next_state = { .p={ .i=next_i, .j=next_j }, .f=curr.s.f };
            if (set_add(visited, next_state)) {
                heap_insert(h, (struct path) { .score=curr.score + 1, .s=next_state });
            }
        }

        // Rotating
        struct state rotated = curr.s;

        rotated.f = ROTATE_CLOCKWISE[curr.s.f];
        if (set_add(visited, rotated)) {
            heap_insert(h, (struct path) { .score=curr.score + 1000, .s=rotated });
        }

        rotated.f = ROTATE_COUNTER_C[curr.s.f];
        if (set_add(visited, rotated)) {
            heap_insert(h, (struct path) { .score=curr.score + 1000, .s=rotated });
        }
    }
}

u8 set_add(u8 set[SET_SIZE], struct state state) {
    u16 index = ((u16)state.p.i << 8) | state.p.j;
    u8 new_content = 1 << state.f;
    u8 old_content = set[index];
    set[index] |= new_content;
    return !(new_content & old_content);
}

// p1 is smaller ->  1;
// p2 is smaller -> -1;
i8 compare_path(struct path p1, struct path p2) {
    return p1.score < p2.score ? 1 : p2.score < p1.score ? -1 : 0;
}

void sink(struct heap *h) {
    u16 index = 1, child = 2, size = h->size;
    struct path sinking = h->paths[index];

    while (child <= size)
    {
        // Get the smaller of the two children
        child += (child < size)
              && (compare_path(h->paths[child], h->paths[child + 1]) < 0);

        // End when the element has a lower score than both its children
        if (compare_path(sinking, h->paths[child]) >= 0) {
            break;
        }

        // Swap the element with the child with the smaller score
        h->paths[index] = h->paths[child];
        h->paths[child] = sinking;
        index = child;
        child <<= 1;
    }
}

struct path heap_pop(struct heap *h) {
    struct path p = h->paths[1];
    h->paths[1] = h->paths[h->size--];
    sink(h);
    return p;
}

void swim(struct heap *h) {
    u16 index = h->size;
    u16 parent = index >> 1;
    struct path swimming = h->paths[index];

    while (parent > 0
        && (compare_path(swimming, h->paths[parent]) > 0))
    {
        h->paths[index] = h->paths[parent];
        h->paths[parent] = swimming;
        index = parent;
        parent >>= 1;
    }
}

void heap_insert(struct heap *h, struct path p) {
    h->paths[++h->size] = p;
    swim(h);
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
