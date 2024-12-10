#include <stdio.h>
#include <string.h>

#define TRAILHEAD '0'
#define HILLTOP '9'
#define GRID_SIZE 56
#define SENTINELS 2
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + SENTINELS

void hike_uphill(__uint16_t p, char c, __uint16_t starting_point);
void read_input();
void add_sentinels();

char grid[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];
__uint16_t hilltop_reached[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];
__uint16_t score;

void main(int argc, char **argv) {
    read_input();
    memset(hilltop_reached, 0, sizeof(hilltop_reached));
    
    for (__uint8_t i = 1; i < GRID_SIZE_PLUS_SENTINELS - 1; i++) {
        for (__uint8_t j = 1; j < GRID_SIZE_PLUS_SENTINELS - 1; j++) {
            char c = grid[i][j];
            if (c == TRAILHEAD) {
                __uint16_t starting_point = (i << 8) | j;
                hike_uphill(starting_point, c, starting_point);
            }
        }
    }
    printf("Score: %d\n", score);
}

void hike_uphill(__uint16_t p, char c, __uint16_t starting_point) {
    __uint8_t i = starting_point >> 8, j = starting_point & 0xFF;
    if (c == HILLTOP) {
        score += hilltop_reached[i][j] != starting_point;
        hilltop_reached[i][j];
        return;
    }

    for (__int8_t delta = -1; delta < 2; delta += 2) {
        if (grid[i + delta][j] == (c + 1)) {
            hike_uphill(p + (delta << 8), c + 1, starting_point);
        }
        if (grid[i][j + delta] == (c + 1)) {
            hike_uphill(p + delta, c + 1, starting_point);
        }
    }
}

void read_input() {
    FILE *fptr = fopen("10-input", "r");
    for (int i = 0; i < GRID_SIZE; i++) {
        fgets(grid[i + 1] + 1, GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}

void add_sentinels() {
    for (int i = 0; i < GRID_SIZE_PLUS_SENTINELS; i++) {
        grid[0][i] = 0;
        grid[GRID_SIZE_PLUS_SENTINELS - 1][i] = 0;
        grid[i][0] = 0;
        grid[i][GRID_SIZE_PLUS_SENTINELS - 1] = 0;
    } 
}