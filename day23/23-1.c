#include <stdio.h>

#define INPUT_SIZE 3380
#define LINE_LENGTH 7
#define GRAPH_SIZE ('z' - 'a' + 1)

typedef unsigned char u8;

short intersection(u8 **t_neighbors, u8 ** n_neighbors);

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
    short three_sets = 0;
    for (u8 i = 0; i < GRAPH_SIZE; i++)
    {
        u8 **t_neighbors = (u8**)adj_lists['t' - 'a'][i];
        for (u8 j = 0; j < GRAPH_SIZE; j++)
        {
            for (u8 k = 0; k < GRAPH_SIZE; k++)
            {
                u8 **n_neighbors = (u8**)adj_lists[j][k];
                three_sets += intersection(t_neighbors, n_neighbors);
            }   
        }
    }
    printf("t Three set count: %hd", three_sets >> 1);
}

short intersection(u8 **t_neighbors, u8 ** n_neighbors) {
    short count = 0;
    for (u8 i = 0; i < GRAPH_SIZE; i++)
    {
        for (u8 j = 0; j < GRAPH_SIZE; j++)
        {
            count += t_neighbors[i][j] & n_neighbors[i][j];
        }
    }
    return count;
}
