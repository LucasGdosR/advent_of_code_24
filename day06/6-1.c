#include <stdio.h>

#define GRID_SIZE 130
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2
#define OBSTACLE '#'
#define NOT_VISITED '.'
#define DIRECTIONS 4

enum Facing {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct point {
    int x;
    int y;
};

struct guard {
    enum Facing facing;
    struct point p;
};

struct guard find_guard();
int within_bounds(struct point p);
void move(struct guard *guard, int *visited);
void read_input();
void add_sentinels();

char input[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

void main(int argc, char const *argv[]) {
    read_input();
    add_sentinels();
    struct guard guard = find_guard();
    
    int visited = 1;
    while (within_bounds(guard.p)) {
        move(&guard, &visited);
    }
    printf("Unique tiles visited: %d\n", visited);
}

struct guard find_guard() {
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j < GRID_SIZE; j++) {
            if (input[i][j] == '^')
                return (struct guard) { .facing = UP, .p = (struct point) { .x = j, .y = i } };
        }
    }
}

int within_bounds(struct point p) {
    return (p.x != 0) && (p.x != GRID_SIZE_PLUS_SENTINELS - 1)
        && (p.y != 0) && (p.y != GRID_SIZE_PLUS_SENTINELS - 1);
}

void move(struct guard *guard, int *visited) {
    struct point destination = guard->p;
    switch (guard->facing) {
        case UP: {destination.y--; break;}
        case RIGHT: {destination.x++; break;}
        case DOWN: {destination.y++; break;}
        case LEFT: {destination.x--; break;}
    }
    switch (input[destination.y][destination.x])
    {
    case OBSTACLE:
        guard->facing = (guard->facing + 1) % DIRECTIONS;
        break;
    case NOT_VISITED:
        input[destination.y][destination.x] = 'X';
        (*visited)++;
    default:
        guard->p = destination;
    } 
}

void read_input() {
    FILE *fptr = fopen("6-input", "r");
    for (int i = 0; i < GRID_SIZE; i++) {
        fgets(input[i + 1] + 1, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}

void add_sentinels() {
    for (int i = 0; i < GRID_SIZE_PLUS_SENTINELS; i++) {
        input[0][i] = 0;
        input[GRID_SIZE_PLUS_SENTINELS - 1][i] = 0;
        input[i][0] = 0;
        input[i][GRID_SIZE_PLUS_SENTINELS - 1] = 0;
    } 
}
