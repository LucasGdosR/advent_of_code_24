#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_SIZE 50
#define ASCII_MAX 255
#define INITIAL_CAPACITY 4

struct p {
    int x;
    int y;
};

struct vec {
    struct p *ps;
    int size;
    int capacity;
};

void append(char key, struct p e);

void init();
void read_input();

char input[GRID_SIZE][GRID_SIZE + 1];
bool antinodes[GRID_SIZE][GRID_SIZE];
struct vec dict[ASCII_MAX];

void main(int argc, char const *argv[]) {
    init();

    // Build a dict by iterating through the input,
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            // putting chars different from . as keys,
            char c = input[i][j];
            if (c != '.') {
            // and the values are lists of their coordinates
                append(c, (struct p) { .x = j, .y = i });
            }
        }    
    }

    // For each key in the dictionary,
    for (int i = 0; i < ASCII_MAX; i++) {
        // iterate through the coordinates in the list,
        struct vec v = dict[i];
        for (int i = 0; i < v.size; i++) {
            struct p p1 = v.ps[i];
            // comparing one with all that come after it
            for (int j = i + 1; j < v.size; j++) {
                struct p p2 = v.ps[j];
                
                // Do some math and fill in antinodes
                struct p small_x = p1.x < p2.x ? p1 : p2;
                struct p big_x = p1.x >= p2.x ? p1 : p2;

                int delta_x = big_x.x - small_x.x;
                int delta_y = big_x.y - small_x.y;

                if (delta_x == 0 || delta_y == 0) {
                    if (delta_x == 0)
                        for (int i = 0; i < GRID_SIZE; i++)
                            antinodes[i][p1.x] = true;
                    if (delta_y == 0)
                        for (int i = 0; i < GRID_SIZE; i++)
                            antinodes[p1.y][i] = true;
                    break;
                }
                
                if (delta_x == delta_y) {
                    delta_x = 1;
                    delta_y = 1;
                }

                struct p p = small_x;
                while (0 <= p.x && p.x < GRID_SIZE && 0 <= p.y  && p.y < GRID_SIZE) {
                    antinodes[p.y][p.x] = true;
                    p.x -= delta_x;
                    p.y -= delta_y;
                }
            
                p = small_x;
                while (0 <= p.x && p.x < GRID_SIZE && 0 <= p.y  && p.y < GRID_SIZE) {
                    antinodes[p.y][p.x] = true;
                    p.x += delta_x;
                    p.y += delta_y;
                }
            }
        }
    }
    
    // Reduce
    int antinode_count = 0;
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            antinode_count += antinodes[i][j];
    
    printf("Antinode count: %d\n", antinode_count);
}

void append(char key, struct p e)
{
    struct vec v = dict[key];
    if (v.size == v.capacity)
    {
        v.capacity = INITIAL_CAPACITY + (v.capacity << 1);
        v.ps = realloc(v.ps, v.capacity * sizeof(struct p));
    }
    v.ps[v.size++] = e;
    dict[key] = v;
}

void init()
{
    read_input();
    memset(antinodes, 0, sizeof(antinodes));
    memset(dict, 0, sizeof(dict));
}

void read_input()
{
    FILE *fptr = fopen("8-input", "r");
    for (int i = 0; i < GRID_SIZE; i++)
    {
        fgets(input[i], GRID_SIZE + 2, fptr);
    }
    fclose(fptr);
}
