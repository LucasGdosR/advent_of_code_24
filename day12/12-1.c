#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 140
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2
#define ARENA_UPPER_BOUND (GRID_SIZE * GRID_SIZE)
#define REGION_ID (ARENA_UPPER_BOUND - 1)
#define REGION_SIZE 0

char garden[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];
bool visited[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

typedef __uint8_t u8;
typedef __uint16_t u16;
typedef __uint32_t u32;

union region {
    typedef struct {
            u8 i;
            u8 j;
    } p;
    u16 size;
    u16 id;
};

void expand_region(char plant, u8 i, u8 j, union region (*region)[ARENA_UPPER_BOUND]);
u32 calculate_perimeter(union region (*region)[ARENA_UPPER_BOUND]);

uint budget_garden_fences() {
    u32 budget = 0;
    union region region[ARENA_UPPER_BOUND];
    u8 region_id = 0;
    for (u8 i = 1; i < GRID_SIZE_PLUS_SENTINELS - 1; i++)
    {
        for (u8 j = 1; j < GRID_SIZE_PLUS_SENTINELS - 1; j++)
        {
            if (!visited[i][j]) {
                visited[i][j] = ++region[REGION_ID].id;
                region[REGION_SIZE].size = 0;
                region[++(region[REGION_SIZE].size)].p = (p) { .i = i, .j = j };
                expand_region(garden[i][j], i, j, &region);
                u32 perimeter = calculate_perimeter(&region);
                budget += perimeter * region[REGION_SIZE].size;
            }
        }
    }
    return budget;
}

void expand_region(char plant, u8 i, u8 j, union region (*region)[ARENA_UPPER_BOUND]) {
#define CHECK_NEIGHBOR(di, dj) \
    if (garden[i + (di)][j + (dj)] == plant && (visited[i + (di)][j + (dj)] != region[REGION_ID].id)) { \
        visited[i + (di)] = region[REGION_ID].id; \
        region[++(region[REGION_SIZE].size)].p = (p) { .i = i + (di), .j = j + (dj) }; \
        expand_region(plant, i + (di), j + (dj), region); \ 
    }
    CHECK_NEIGHBOR(1, 0)
    CHECK_NEIGHBOR(-1, 0)
    CHECK_NEIGHBOR(0, 1)
    CHECK_NEIGHBOR(0, -1)
}

u32 calculate_perimeter(union region (*region)[ARENA_UPPER_BOUND]) {
    u32 perimeter = 0;
    for (u16 i = 1,
             id = region[REGION_ID].id,
             size = region[REGION_SIZE].size;
        i <= size; i++)
    {
        p p = region[i].p;
        i = p.i;
        j = p.j;
        perimeter += (visited[i + 1][j] != id)
                   + (visited[i - 1][j] != id)
                   + (visited[i][j + 1] != id)
                   + (visited[i][j - 1] != id);
    }
    return perimeter;
}

void read_input() {
    char buffer[GRID_SIZE];
    FILE *fptr = fopen("10-input", "r");
    for (u8 i = 0; i < GRID_SIZE; i++)
    {
        fgets(garden[i + 1] + 1, GRID_SIZE + 2, fptr)
    }
    fclose(fptr);
}

void main(int argc, char const *argv[])
{
    read_input();
    printf("Budget: %d\n", budget_garden_fences());
}
