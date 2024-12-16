#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 140
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2
#define ARENA_UPPER_BOUND (GRID_SIZE * GRID_SIZE)
#define REGION_ID (ARENA_UPPER_BOUND - 1)
#define REGION_ID_INCREMENT 2
#define ID_MASK 0xFFFE
#define CORNER_MASK 1
#define REGION_SIZE 0

typedef __uint8_t u8;
typedef __uint16_t u16;
typedef __uint32_t u32;

char garden[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];
u16 visited[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

typedef struct {
    u8 i;
    u8 j;
} p;

union region {
    p p;
    u16 size;
    u16 id;
};

void expand_region(char plant, u8 i, u8 j, union region region[ARENA_UPPER_BOUND]);
u32 calculate_sides(union region region[ARENA_UPPER_BOUND]);

uint budget_garden_fences() {
    u32 budget = 0;
    union region region[ARENA_UPPER_BOUND];
    region[REGION_ID].id = 0;
    for (u8 i = 1; i < GRID_SIZE_PLUS_SENTINELS - 1; i++)
    {
        for (u8 j = 1; j < GRID_SIZE_PLUS_SENTINELS - 1; j++)
        {
            if (!visited[i][j]) {
                region[REGION_ID].id += REGION_ID_INCREMENT;
                visited[i][j] = region[REGION_ID].id;
                region[REGION_SIZE].size = 0;
                region[++(region[REGION_SIZE].size)].p = (p) { .i = i, .j = j };
                expand_region(garden[i][j], i, j, region);
                u32 sides = calculate_sides(region);
                budget += sides * region[REGION_SIZE].size;
            }
        }
    }
    return budget;
}

void expand_region(char plant, u8 i, u8 j, union region region[ARENA_UPPER_BOUND]) {
#define CHECK_NEIGHBOR(di, dj) \
    if (garden[i + (di)][j + (dj)] == plant \
    && (visited[i + (di)][j + (dj)] != region[REGION_ID].id)) \
    { \
        visited[i + (di)][j + (dj)] = region[REGION_ID].id; \
        region[++(region[REGION_SIZE].size)].p = (p) { .i = i + (di), .j = j + (dj) }; \
        expand_region(plant, i + (di), j + (dj), region); \
    }
    CHECK_NEIGHBOR(1, 0)
    CHECK_NEIGHBOR(-1, 0)
    CHECK_NEIGHBOR(0, 1)
    CHECK_NEIGHBOR(0, -1)
}

u32 calculate_sides(union region region[ARENA_UPPER_BOUND]) {
    u32 sides = 0;
    u16 size = region[REGION_SIZE].size;

    u16 min_i = GRID_SIZE_PLUS_SENTINELS;
    u16 min_j = GRID_SIZE_PLUS_SENTINELS;
    u16 max_i = 0, max_j = 0;

    for (u16 i = 1, id = region[REGION_ID].id; i <= size; i++)
    {
        p p = region[i].p;

        sides += 2 * (((visited[p.i+1][p.j+1] & ID_MASK) == id)
                   && ((visited[p.i+1][p.j  ] & ID_MASK) != id)
                   && ((visited[p.i  ][p.j+1] & ID_MASK) != id))
               + 2 * (((visited[p.i+1][p.j-1] & ID_MASK) == id)
                   && ((visited[p.i+1][p.j  ] & ID_MASK) != id)
                   && ((visited[p.i  ][p.j-1] & ID_MASK) != id));

        visited[p.i][p.j] ^= CORNER_MASK;
        visited[p.i+1][p.j] ^= CORNER_MASK;
        visited[p.i][p.j+1] ^= CORNER_MASK;
        visited[p.i+1][p.j+1] ^= CORNER_MASK;

        min_i = min_i < p.i ? min_i : p.i;
        min_j = min_j < p.j ? min_j : p.j;
        max_i = max_i > p.i+1 ? max_i : p.i+1;
        max_j = max_j > p.j+1 ? max_j : p.j+1;
    }
    for (u16 i = min_i; i <= max_i; i++)
    {
        for (u16 j = min_j; j <= max_j; j++)
        {
            sides += visited[i][j] & CORNER_MASK;
            visited[i][j] &= ID_MASK;
        }        
    }
    return sides;
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
