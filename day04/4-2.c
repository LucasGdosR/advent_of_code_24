#include <stdio.h>

#define GRID_SIZE 140

struct point {
    int x;
    int y;
};

void read_input();
int find_MAS(int x, int y);
int is_cross(struct point p1, struct point p2);

char input[GRID_SIZE][GRID_SIZE];

void main(int argc, char const *argv[]) {
    read_input();
    int count = 0;
    for (int i = 1; i < GRID_SIZE - 1; i++) {
        for (int j = 1; j < GRID_SIZE - 1; j++) {
            if (input[i][j] == 'A') {
                count += find_MAS(i, j);
            }
        }    
    }
    printf("X-MAS count: %d\n", count);
}

int find_MAS(int x, int y) {
    return is_cross((struct point) {.x = x - 1, .y = y - 1},
                    (struct point) {.x = x + 1, .y = y + 1})
        && is_cross((struct point) {.x = x + 1, .y = y - 1},
                    (struct point) {.x = x - 1, .y = y + 1});
}

int is_cross(struct point p1, struct point p2) {
    return (input[p1.x][p1.y] == 'M' && input[p2.x][p2.y] == 'S')
        || (input[p1.x][p1.y] == 'S' && input[p2.x][p2.y] == 'M');
}

void read_input() {
    FILE *fptr = fopen("4-input", "r");
    for (int i = 0; i < GRID_SIZE; i++) {
        fgets(input[i], GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}
