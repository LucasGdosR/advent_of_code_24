#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 3380
#define LINE_LENGTH 7
#define ALPHABET ('z' - 'a' + 1)
#define GRAPH_SIZE (ALPHABET * ALPHABET)
#define BITSET_SIZE ((GRAPH_SIZE - 1) / 64 + 1)
#define NETWORK_SIZE 520

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u64;

typedef u64 bitset[BITSET_SIZE];

struct network {
    bitset *pcs_neighbors[GRAPH_SIZE];
    bitset arena[NETWORK_SIZE];
    u16 pcs_connected;
};

struct network network;

void set_union(bitset p, bitset x, bitset result) {
    for (u8 i = 0; i < BITSET_SIZE; i++)
    {
        result[i] = p[i] | x[i];
    }
}

void set_intersection(bitset p, u16 pc, bitset result) {
    bitset *set = network.pcs_neighbors[pc];
    
    for (u8 i = 0; i < BITSET_SIZE; i++)
    {
        result[i] = p[i] & (*set)[i];
    }
}

void set_difference(bitset p, bitset *d, bitset result) {
    for (u8 i = 0; i < BITSET_SIZE; i++)
    {
        result[i] = p[i] & ~(*d)[i];
    }
}

void set_add(bitset s, u16 pc) {
    u8 index = pc >> 6;
    u8 shift = pc & 63;
    s[index] |= 1UL << shift;
}

void set_remove(bitset s, u16 pc) {
    u8 index = pc >> 6;
    u8 shift = pc & 63;
    s[index] &= ~(1UL << shift);
}

u64 set_has(bitset s, u16 pc) {
    return s[pc >> 6] & (1UL << (pc & 63));
}

short set_len(bitset s) {
    short len = 0;
    for (u8 i = 0; i < BITSET_SIZE; i++)
    {
        u64 n = s[i];
        // Brian Kernighan's algorithm for counting 1s
        while (n)
        {
            n &= n - 1;
            len++;
        }
    }
    return len;
}

u16 get_pivot(bitset p, bitset x) {
    u16 pivot;
    short max_connections = -1;
    bitset u;
    set_union(p, x, u);

    for (u16 pc = 0; pc < GRAPH_SIZE; pc++)
    {
        if (set_has(u, pc))
        {
            bitset i;
            set_intersection(p, pc, i);
            short connections = set_len(i);
            if (connections > max_connections)
            {
                max_connections = connections;
                pivot = pc;
            }
        }
    }
    
    return pivot;
}

void bron_kerbosch(bitset r, bitset p, bitset x, bitset max_clique) {
    if ((!set_len(p)) && (!set_len(x)))
    {
        if (set_len(r) > set_len(max_clique))
        {
            memcpy(max_clique, r, sizeof(bitset));
        }
    }
    else
    {
        u16 pivot = get_pivot(p, x);
        bitset *pivot_neighbors = network.pcs_neighbors[pivot];
        bitset d;
        set_difference(p, pivot_neighbors, d);

        for (u16 pc = 0; pc < GRAPH_SIZE; pc++)
        {
            if (set_has(d, pc))
            {
                bitset new_r, new_p, new_x;
                memcpy(new_r, r, sizeof(bitset));
                set_add(new_r, pc);
                set_intersection(p, pc, new_p);
                set_intersection(x, pc, new_x);
                bron_kerbosch(new_r, new_p, new_x, max_clique);
                set_remove(p, pc);
                set_add(x, pc);
            }
        }
    }
}

void add_edge(u16 node1, u16 node2) {
    if (network.pcs_neighbors[node1] == NULL)
    {
        network.pcs_neighbors[node1] = &network.arena[network.pcs_connected++];
    }
    u8 index = node2 >> 6;
    u8 shift = node2 & 63;
    (*network.pcs_neighbors[node1])[index] |= 1UL << shift;
}

void read_input(bitset all_vertices) {
    FILE *f = fopen("23-input", "r");
    char buffer[LINE_LENGTH];
    for (u16 i = 0; i < INPUT_SIZE; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        u16 pc1 = (buffer[0] - 'a') * ALPHABET + buffer[1] - 'a';
        u16 pc2 = (buffer[3] - 'a') * ALPHABET + buffer[4] - 'a';
        add_edge(pc1, pc2);
        add_edge(pc2, pc1);
        set_add(all_vertices, pc1);
        set_add(all_vertices, pc2);
    }
    fclose(f);
}

void main(int argc, char const *argv[])
{
    bitset all_vertices = {}, max_clique = {}, r = {}, x = {};
    read_input(all_vertices);
    bron_kerbosch(r, all_vertices, x, max_clique);
    printf("Largest clique: ");
    for (u16 pc = 0; pc < GRAPH_SIZE; pc++)
    {
        if (set_has(max_clique, pc))
        {
            char a = (pc / ALPHABET) + 'a';
            char b = (pc % ALPHABET) + 'a';
            printf("%c%c,", a, b);
        }
    }
    printf("\n");
}
