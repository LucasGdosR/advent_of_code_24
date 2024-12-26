#include <stdio.h>

#define MAZE_SIZE 141
#define HEAP_SIZE 1024
#define HEAP_START 1
#define SET_SIZE MAZE_SIZE - 2
#define MAP_CAPACITY 52500
#define MAP_SIZE 70001 // prime number > MAP_CAPACITY / 0.75

typedef signed   char  i8 ;
typedef unsigned char  u8 ;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

struct p {
    u8 i;
    u8 j;
};

enum Facing                            {  EAST ,  SOUTH,  WEST  ,  NORTH, NUM_FACINGS  };
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

struct val {
    struct state prev_neighbors[3];
    u8 size;
    u32 score;
};

struct node {
    struct state key;
    struct val v;
    struct node *next;
};

struct map {
    struct node *arr[MAP_SIZE];
    struct node arena[MAP_CAPACITY];
    u16 size;
};


void find_shortest_path(char maze[MAZE_SIZE][MAZE_SIZE + 2], struct heap *h, struct map *m);
short count_tiles_in_path(struct map *tile_to_prev_neighbors, struct p end);
void find_start_end(char maze[MAZE_SIZE][MAZE_SIZE + 2], struct p start_end[2]);

u8 set_add(u8 set[SET_SIZE][SET_SIZE], struct state state);

struct path heap_pop(struct heap *h);
void heap_insert(struct heap *h, struct path p);

void map_insert(struct map *m, struct state k, struct val v);
struct val* map_get(struct map *m, struct state this);

void read_input(char maze[MAZE_SIZE][MAZE_SIZE + 2]);

void main(int argc, char const *argv[])
{
    char maze[MAZE_SIZE][MAZE_SIZE + 2];
    read_input(maze);

    struct p start_end[2] = {};
    find_start_end(maze, start_end);
    struct state beginning = { .p=start_end[0], .f=EAST };

    struct heap h;
    h.size = 1, h.paths[HEAP_START] = (struct path) { .score=0, .s=beginning };

    struct map tile_to_prev_neighbors;
    struct val v = {};
    map_insert(&tile_to_prev_neighbors, beginning, v);

    find_shortest_path(maze, &h, &tile_to_prev_neighbors);

    short tiles = count_tiles_in_path(&tile_to_prev_neighbors, start_end[1]);

    printf("Tiles in all shortest paths: %hd\n", tiles);
}

void find_shortest_path(char maze[MAZE_SIZE][MAZE_SIZE + 2], struct heap *h, struct map *m) {
    u32 max_score = __UINT32_MAX__, score;
    u8 visited[SET_SIZE][SET_SIZE] = {};
    set_add(visited, h->paths[HEAP_START].s);

    do
    {
        struct path curr = heap_pop(h);
        score = curr.score;
        
        // Trying to move forward
        struct p d = DIRECTIONS[curr.s.f];
        u8 next_i = curr.s.p.i + d.i, next_j = curr.s.p.j + d.j;
        struct state next_state = { .p={ .i=next_i, .j=next_j }, .f=curr.s.f };

        struct val *v;
        if ((v = map_get(m, next_state)) == NULL)
        {
            struct val new_v = { .prev_neighbors={ curr.s }, .size=1, .score=curr.score + 1 };
            map_insert(m, next_state, new_v);
        }
        else if (v->score == curr.score + 1)
        {
            v->prev_neighbors[v->size++] = curr.s;
        }

        char tile = maze[next_i][next_j];
        if (tile == 'E') {
            max_score = curr.score + 1;
        } else if (tile == '.') {
            if (set_add(visited, next_state)) {
                heap_insert(h, (struct path) { .score=curr.score + 1, .s=next_state });
            }
        }

        // Rotating
        #define TRY_ROTATING if ((v = map_get(m, rotated)) == NULL) \
        { \
            struct val new_v = { .prev_neighbors={ curr.s }, .size=1, .score=curr.score + 1000 }; \
            map_insert(m, rotated, new_v); \
        } \
        else if (v->score == curr.score + 1000) \
        { \
            v->prev_neighbors[v->size++] = curr.s; \
        } \
\
        if (set_add(visited, rotated)) { \
            heap_insert(h, (struct path) { .score=curr.score + 1000, .s=rotated }); \
        }
        struct state rotated = curr.s;
        rotated.f = ROTATE_CLOCKWISE[curr.s.f];
        TRY_ROTATING
        rotated.f = ROTATE_COUNTER_C[curr.s.f];
        TRY_ROTATING
    } while (max_score > score);
}

void backtrack_tiles(struct map *m, struct state s, char tiles[MAZE_SIZE][MAZE_SIZE]) {
    tiles[s.p.i][s.p.j] = 1;
    struct val *v = map_get(m, s);
    for (u8 i = 0; i < v->size; i++)
    {
        backtrack_tiles(m, v->prev_neighbors[i], tiles);
    }
}

short count_tiles_in_path(struct map *tile_to_prev_neighbors, struct p end) {
    char tiles[MAZE_SIZE][MAZE_SIZE] = {};

    for (enum Facing f = 0; f < NUM_FACINGS; f++)
    {
        struct state s = { .p=end, .f=f };
        if (map_get(tile_to_prev_neighbors, s))
        {
            backtrack_tiles(tile_to_prev_neighbors, s, tiles);
        }
    }

    short count = 0;
    for (u8 i = 0; i < MAZE_SIZE; i++)
    {
        for (u8 j = 0; j < MAZE_SIZE; j++)
        {
            count += tiles[i][j];
        }
    }
    return count;
}

u8 set_add(u8 set[SET_SIZE][SET_SIZE], struct state state) {
    u8 new_content = 1 << state.f;
    u8 old_content = set[state.p.i - 1][state.p.j - 1];
    set[state.p.i - 1][state.p.j - 1] |= new_content;
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

u16 map_hash(struct state k) {
    u64 h1 = (k.p.i * 31415) + 27183;
    u64 h2 = (k.p.j * 27183) + 18979;
    u64 h3 = (k.f * 18979) + 31415;
    u64 hash_value = h1 * 65537 + h2 * 65539 + h3 * 65543;
    hash_value = ((hash_value >> 16) ^ hash_value) * 65537;
    return hash_value % MAP_SIZE;
}

void map_insert(struct map *m, struct state k, struct val v) {
    u16 h = map_hash(k);
    struct node node = { .key=k, .v=v, .next=m->arr[h] };
    m->arena[m->size] = node;
    m->arr[h] = &m->arena[m->size++];
}

struct val* map_get(struct map *m, struct state this) {
    struct val *v = NULL;
    u16 h = map_hash(this);
    struct node *node = m->arr[h];
    while (node)
    {
        struct state that = node->key;
        if ((this.p.i == that.p.i)
         && (this.p.j == that.p.j)
         && (this.f == that.f))
        {
            v = &node->v;
            break;
        }
        node = node->next;
    }
    return v;
}

void find_start_end(char maze[MAZE_SIZE][MAZE_SIZE + 2], struct p start_end[2]) {
    for (u8 i = 0; i < MAZE_SIZE; i++)
    {
        for (u8 j = 0; j < MAZE_SIZE; j++)
        {
            char tile = maze[i][j];
            if (tile == 'S')
            {
                start_end[0] = (struct p) { .i=i, .j=j };
                if (start_end[1].i)
                {
                    return;
                }
            }
            if (tile == 'E')
            {
                start_end[1] = (struct p) { .i=i, .j=j };
                if (start_end[0].i)
                {
                    return;
                }
            }
        }
    }
}

void read_input(char maze[MAZE_SIZE][MAZE_SIZE + 2]) {
    FILE *f = fopen("16-input", "r");
    for (u8 i = 0; i < MAZE_SIZE; i++)
    {
        fgets(maze[i], MAZE_SIZE + 2, f);
    }
    fclose(f);
}
