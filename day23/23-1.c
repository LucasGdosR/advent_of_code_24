#include <stdio.h>

#define INPUT_SIZE 3380
#define LINE_LENGTH 7
#define GRAPH_SIZE ('z' - 'a' + 1)
#define HASH_SET_SIZE 1230
#define HASH_SET_CAPACITY 1657 // prime > size / 0.75

typedef unsigned char u8;

struct set_of_three {
    short small;
    short middle;
    short large;
};

struct three_sets {
    struct set_of_three *sets[HASH_SET_CAPACITY];
    struct set_of_three arena[HASH_SET_SIZE];
    short size;
};

void add_intersection(struct three_sets *three_sets, u8 (*t_neighbors)[GRAPH_SIZE], u8 (*n_neighbors)[GRAPH_SIZE]);

// TODO: make these pointers. Instantiate arrays lazily, like a hierarchical page table.
u8 adj_lists[GRAPH_SIZE][GRAPH_SIZE][GRAPH_SIZE][GRAPH_SIZE];

void read_input() {
    FILE *f = fopen("23-input", "r");
    char buffer[LINE_LENGTH];
    for (short i = 0; i < INPUT_SIZE; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        adj_lists
        [buffer[0] - 'a']
        [buffer[1] - 'a']
        [buffer[3] - 'a']
        [buffer[4] - 'a']
        = 1;
        adj_lists
        [buffer[3] - 'a']
        [buffer[4] - 'a']
        [buffer[0] - 'a']
        [buffer[1] - 'a']
        = 1;
    }
    fclose(f);
}

void main(int argc, char const *argv[])
{
    read_input();
    struct three_sets three_sets = {};
    for (u8 i = 0; i < GRAPH_SIZE; i++)
    {
        u8 (*t_neighbors)[GRAPH_SIZE] = adj_lists['t' - 'a'][i];
        for (u8 j = 0; j < GRAPH_SIZE; j++)
        {
            for (u8 k = 0; k < GRAPH_SIZE; k++)
            {
                u8 (*n_neighbors)[GRAPH_SIZE] = adj_lists[j][k];
                add_intersection(&three_sets, t_neighbors, n_neighbors);
            }   
        }
    }
    printf("t three set count: %hd", three_sets.size);
}

short intersection(u8 (*t_neighbors)[GRAPH_SIZE], u8 (*n_neighbors)[GRAPH_SIZE]) {
    
}

void add_intersection(struct three_sets *three_sets, u8 (*t_neighbors)[GRAPH_SIZE], u8 (*n_neighbors)[GRAPH_SIZE]) {
    for (u8 i = 0; i < GRAPH_SIZE; i++)
    {
        for (u8 j = 0; j < GRAPH_SIZE; j++)
        {
            if (t_neighbors[i][j] & n_neighbors[i][j]) {
                // add set of three to three sets
            }
        }
    }
}
