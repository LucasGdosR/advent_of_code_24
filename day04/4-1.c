#include <stdio.h>

#define GRID_SIZE 140
#define GRID_SIZE_PLUS_SENTINELS GRID_SIZE + 2

void read_input();
void add_sentinels();
int find_MAS(int x, int y);

char input[GRID_SIZE_PLUS_SENTINELS][GRID_SIZE_PLUS_SENTINELS];

void main(int argc, char const *argv[]) {
    read_input();
    add_sentinels();

    int count = 0;
    for (int i = 1; i <= GRID_SIZE; i++) {
        for (int j = 1; j <= GRID_SIZE; j++) {
            if (input[i][j] == 'X') {
                count += find_MAS(i, j);
            }
        }    
    }
    printf("XMAS count: %d\n", count);
}

const int DIRECTIONS[8][2] = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1,  0},          {1,  0},
    {-1,  1}, {0,  1}, {1,  1}
};

int find_MAS(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int dx = DIRECTIONS[i][0], dy = DIRECTIONS[i][1];
        if (input[x + 1*dx][y + 1*dy] == 'M' &&
            input[x + 2*dx][y + 2*dy] == 'A' &&
            input[x + 3*dx][y + 3*dy] == 'S')
        count++;
    }
    return count;
}

void read_input() {
    FILE *fptr = fopen("4-input", "r");
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
