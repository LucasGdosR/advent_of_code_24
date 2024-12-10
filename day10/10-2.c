#include <stdio.h>

#define TRAILHEAD '0'
#define HILLTOP '9'
#define GRID_SIZE 56
#define SENTINELS 2
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + SENTINELS

__uint16_t hike_uphill(__uint8_t i, __uint8_t j, char c);
void read_input();

char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

void main(int argc, char **argv) {
    read_input();
    
    __uint16_t ratings_sum = 0;
    for (__uint8_t i = 1; i < GRID_SIZE_PLUS_SENTINELS - 1; i++) {
        for (__uint8_t j = 1; j < GRID_SIZE_PLUS_SENTINELS - 1; j++) {
            char c = grid[i][j];
            if (c == TRAILHEAD) {
                ratings_sum += hike_uphill(i, j, c);
            }
        }
    }
    printf("Ratings sum: %d\n", ratings_sum);
}

__uint16_t hike_uphill(__uint8_t i, __uint8_t j, char c) {
    if (c == HILLTOP) {
        return 1;
    }

    __uint16_t rating = 0;
    for (__int8_t d = -1; d < 2; d += 2) {
        if (grid[i + d][j] == (c + 1)) {
            rating += hike_uphill(i+ d, j, c + 1);
        }
        if (grid[i][j + d] == (c + 1)) {
            rating += hike_uphill(i, j + d, c + 1);
        }
    }
    return rating;
}

void read_input() {
    FILE *fptr = fopen("10-input", "r");
    for (int i = 0; i < GRID_SIZE; i++) {
        fgets(grid[i + 1] + 1, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}
