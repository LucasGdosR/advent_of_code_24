#include <stdio.h>

#define INPUT_SIZE 3380
#define LINE_LENGTH 7
#define GRAPH_SIZE ('z' - 'a' + 1)

adj_lists[GRAPH_SIZE][GRAPH_SIZE][GRAPH_SIZE][GRAPH_SIZE];

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
/*
three_sets = set()
for node in adj_lists:
    if node[0] == 't':
        t_neighbors = adj_lists[node]
        for n in t_neighbors:
            final_computers = t_neighbors.intersection(adj_lists[n])
            for third_computer in final_computers:
                three_sets.add(tuple(sorted((node, n, third_computer))))
print(len(three_sets))
*/
}
