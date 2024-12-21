#include <stdio.h>

#define GRID 141
#define SENTINEL 19
#define SENTINELS (2 * SENTINEL)
#define GRID_PLUS_SENTINELS (GRID + SENTINELS)
#define WALL '#'
#define MAX_CHEAT 20
#define QUEUE_SIZE (MAX_CHEAT * 4) // The queue holds 1 level at a time, and each level has 4 more elements than the last
#define ARENA_CAPACITY 841 // A grid of radius 20 reaches 841 tiles
#define SET_SIZE (1663) // Suitable prime number

typedef unsigned char u8;
typedef signed char i8;

struct p { u8 i, j; };

struct q {
    struct p ps[QUEUE_SIZE];
    u8 start;
    u8 end;
};

struct node {
    struct p p;
    struct node *next;
};

struct set {
    struct node *lists[SET_SIZE];
    struct node arena[ARENA_CAPACITY];
    short size;
};

void enqueue(struct q *q, struct p p);
struct p dequeue(struct q *q);
u8 size(struct q *q);
u8 add(struct set *set, struct p p);

char racetrack[GRID_PLUS_SENTINELS][GRID_PLUS_SENTINELS];
short path[GRID_PLUS_SENTINELS][GRID_PLUS_SENTINELS];
int collisions;

void cache_distances(u8 i, u8 j) {
    short steps = 0;
    u8 keep_going;
    do
    {
        keep_going = 0;
        #define SEARCH_NEIGHBOR(di, dj) if ((racetrack[i+(di)][j+(dj)] != WALL) && (!path[i+(di)][j+(dj)])) { \
            path[i+(di)][j+(dj)] = ++steps; \
            keep_going = 1, i = i+(di), j = j+(dj); \
        }
        SEARCH_NEIGHBOR( 0,  1)
        SEARCH_NEIGHBOR( 0, -1)
        SEARCH_NEIGHBOR( 1,  0)
        SEARCH_NEIGHBOR(-1,  0)
    } while (keep_going);    
}

int find_cheats(struct p start) {
    short steps = path[start.i][start.j];

    struct q q;
    q.start = q.end = 0;
    enqueue(&q, start);

    struct set visited = {};
    add(&visited, start);

    int cheats = 0;
    for (u8 cheating_steps = 0; cheating_steps < 20; cheating_steps++)
    {
        u8 len = size(&q);
        for (u8 _ = 0; _ < len; _++)
        {
            struct p p = dequeue(&q);
            #define SEARCH_CHEAT(di, dj) \
            { \
                struct p dst = (struct p) { .i=p.i+(di), .j=p.j+(dj)}; \
                if (add(&visited, dst)) { \
                    enqueue(&q, dst); \
                    cheats += path[dst.i][dst.j] > (steps + 100 + cheating_steps); \
                } \
            }
            SEARCH_CHEAT( 0,  1)
            SEARCH_CHEAT( 0, -1)
            SEARCH_CHEAT( 1,  0)
            SEARCH_CHEAT(-1,  0)
        }  
    }
    return cheats;
}

void enqueue(struct q *q, struct p p) {
    q->ps[q->end++] = p;
    q->end %= QUEUE_SIZE;
}

struct p dequeue(struct q *q) {
    struct p p = q->ps[q->start++];
    q->start %= QUEUE_SIZE;
    return p;
}

u8 size(struct q *q) {
    return q->end + (q->start > q->end) * QUEUE_SIZE - q->start;
}

u8 add(struct set *set, struct p p) {
    short hash = (p.i * (SENTINEL + GRID) + p.j) % SET_SIZE;
    struct node *head = set->lists[hash];
    struct node *node = head;

    while (node) {
        if (node->p.i == p.i && node->p.j == p.j)
            return 0;
        collisions++;
        node = node->next;
    }
    set->arena[set->size] = (struct node) { .p=p, .next=head };
    set->lists[hash] = &set->arena[set->size++];
    return 1;
}

void read_input();
struct p find_start();

void main(int argc, char const *argv[])
{
    read_input();
    struct p start = find_start();

    path[start.i][start.j] = -1;
    cache_distances(start.i, start.j);
    path[start.i][start.j] = 0;
    
    int cheats = 0;
    for (u8 i = SENTINEL; i < SENTINEL + GRID; i++) {
        for (u8 j = SENTINEL; j < SENTINEL + GRID; j++) {
            if (racetrack[i][j] != WALL) {
                cheats += find_cheats((struct p) { .i=i, .j=j });
            }
        }
    }
    printf("Cheats yielding 100+ improvements: %d\n", cheats);
    printf("Collisions: %d\n", collisions);
}

struct p find_start() {
    for (u8 i = SENTINEL; i < SENTINEL + GRID; i++)
        for (u8 j = SENTINEL; j < SENTINEL + GRID; j++)
            if (racetrack[i][j] == 'S')
                return (struct p) { .i=i, .j=j };
}

void read_input() {
    FILE *f = fopen("20-input", "r");
    for (u8 i = 0; i < GRID; i++) {
        fgets(racetrack[i + SENTINEL] + SENTINEL, GRID + 2, f);
    }
    fclose(f);
}
