#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 140
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2
#define ARENA_UPPER_BOUND (GRID_SIZE * GRID_SIZE)
#define REGION_SIZE 0
#define true 1

typedef __uint8_t u8;
typedef u8 bool;
typedef __uint16_t u16;
typedef __uint32_t u32;

char garden[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];
bool visited[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

typedef struct {
    u8 i;
    u8 j;
} p;

union region {
    p p;
    u16 size;
};

void expand_region(char plant, u8 i, u8 j, union region region[ARENA_UPPER_BOUND]);
u32 calculate_perimeter(union region region[ARENA_UPPER_BOUND]);

uint budget_garden_fences() {
    u32 budget = 0;
    union region region[ARENA_UPPER_BOUND];
    for (u8 i = 1; i < GRID_SIZE_PLUS_SENTINELS - 1; i++)
    {
        for (u8 j = 1; j < GRID_SIZE_PLUS_SENTINELS - 1; j++)
        {
            if (!visited[i][j]) {
                visited[i][j] = true;
                region[REGION_SIZE].size = 0;
                region[++(region[REGION_SIZE].size)].p = (p) { .i = i, .j = j };
                expand_region(garden[i][j], i, j, region);
                u32 perimeter = calculate_perimeter(region);
                budget += perimeter * region[REGION_SIZE].size;
            }
        }
    }
    return budget;
}

void expand_region(char plant, u8 i, u8 j, union region region[ARENA_UPPER_BOUND]) {
#define CHECK_NEIGHBOR(di, dj) \
    if (garden[i + (di)][j + (dj)] == plant \
    && (!visited[i + (di)][j + (dj)])) \
    { \
        visited[i + (di)][j + (dj)] = true; \
        region[++(region[REGION_SIZE].size)].p = (p) { .i = i + (di), .j = j + (dj) }; \
        expand_region(plant, i + (di), j + (dj), region); \
    }
    CHECK_NEIGHBOR(1, 0)
    CHECK_NEIGHBOR(-1, 0)
    CHECK_NEIGHBOR(0, 1)
    CHECK_NEIGHBOR(0, -1)
}

u32 calculate_perimeter(union region region[ARENA_UPPER_BOUND]) {
    u32 perimeter = 0;
    for (u16 i = 1,
             plant = garden[region[1].p.i][region[1].p.j],
             size = region[REGION_SIZE].size;
        i <= size; i++)
    {
        p p = region[i].p;
        perimeter += (garden[p.i + 1][p.j] != plant)
                   + (garden[p.i - 1][p.j] != plant)
                   + (garden[p.i][p.j + 1] != plant)
                   + (garden[p.i][p.j - 1] != plant);
    }
    return perimeter;
}

void read_input() {
    char buffer[GRID_SIZE];
    FILE *fptr = fopen("12-input", "r");
    for (u8 i = 0; i < GRID_SIZE; i++)
    {
        fgets(garden[i + 1] + 1, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}

void main(int argc, char const *argv[])
{
    read_input();
    printf("Budget: %d\n", budget_garden_fences());
}
