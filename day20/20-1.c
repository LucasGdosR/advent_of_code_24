#include <stdio.h>

#define GRID 141
#define GRID_PLUS_SENTINELS (GRID + 2)
#define WALL '#'

typedef unsigned char u8;

struct p { u8 i, j; };

char racetrack[GRID_PLUS_SENTINELS][GRID_PLUS_SENTINELS + 2];
short path[GRID_PLUS_SENTINELS][GRID_PLUS_SENTINELS];

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

u8 find_cheats(u8 i, u8 j) {
    short steps = path[i][j];
    #define SEARCH_CHEAT(di, dj) (path[i+(di)][j+(dj)] >= (steps + 102))
    return  SEARCH_CHEAT( 0,  2) +
            SEARCH_CHEAT( 0, -2) +
            SEARCH_CHEAT( 2,  0) +
            SEARCH_CHEAT(-2,  0) ;
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
    
    short cheats = 0;
    for (u8 i = 1; i < GRID_PLUS_SENTINELS - 1; i++) {
        for (u8 j = 1; j < GRID_PLUS_SENTINELS - 1; j++) {
            if (racetrack[i][j] != WALL) {
                cheats += find_cheats(i, j);
            }
        }
    }
    printf("Cheats yielding 100+ improvements: %hd\n", cheats);
}

struct p find_start() {
    for (u8 i = 1; i < GRID_PLUS_SENTINELS - 1; i++)
        for (u8 j = 1; j < GRID_PLUS_SENTINELS - 1; j++)
            if (racetrack[i][j] == 'S')
                return (struct p) { .i=i, .j=j };
}

void read_input() {
    FILE *f = fopen("20-input", "r");
    for (u8 i = 0; i < GRID; i++) {
        fgets(racetrack[i + 1] + 1, GRID + 2, f);
    }
    fclose(f);
}
