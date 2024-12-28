#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 3380
#define LINE_LENGTH 7
#define ALPHABET ('z' - 'a' + 1)
#define GRAPH_SIZE (ALPHABET * ALPHABET)
#define HASH_SET_SIZE 1230
#define HASH_SET_CAPACITY 1657 // prime > size / 0.75
#define NETWORK_SIZE 520

typedef unsigned char u8;
typedef unsigned short u16;

struct set_of_three { u16 small, middle, large; };

struct node {
    struct set_of_three set;
    struct node *next;
};

struct three_sets {
    struct node *sets[HASH_SET_CAPACITY];
    struct node arena[HASH_SET_SIZE];
    u16 size;
};

struct network {
    u8 (*pcs_neighbors[GRAPH_SIZE])[GRAPH_SIZE];
    u8 arena[NETWORK_SIZE][GRAPH_SIZE];
    u16 pcs_connected;
};

struct network network;

void add_edge(u16 node1, u16 node2) {
    if (network.pcs_neighbors[node1] == NULL)
    {
        network.pcs_neighbors[node1] = &network.arena[network.pcs_connected++];
    }
    (*network.pcs_neighbors[node1])[node2] = 1;
}

void read_input() {
    FILE *f = fopen("23-input", "r");
    char buffer[LINE_LENGTH];
    for (u16 i = 0; i < INPUT_SIZE; i++)
    {
        fgets(buffer, LINE_LENGTH, f);
        u16 pc1 = (buffer[0] - 'a') * ALPHABET + buffer[1] - 'a';
        u16 pc2 = (buffer[3] - 'a') * ALPHABET + buffer[4] - 'a';
        add_edge(pc1, pc2);
        add_edge(pc2, pc1);
    }
    fclose(f);
}

struct set_of_three make_set(u16 foo, u16 bar, u16 baz) {
    struct set_of_three set;
    if (foo < bar) {
        if (foo < baz) {
            if (bar < baz) {
                set = (struct set_of_three) { .small=foo, .middle=bar, .large=baz };
            } else {
                set = (struct set_of_three) { .small=foo, .middle=baz, .large=bar };
            }
        } else {
            set = (struct set_of_three) { .small=baz, .middle=foo, .large=bar };
        }
    } else {
        if (bar < baz) {
            if (foo < baz) {
                set = (struct set_of_three) { .small=bar, .middle=foo, .large=baz };
            } else {
                set = (struct set_of_three) { .small=bar, .middle=baz, .large=foo };
            }
        } else {
            set = (struct set_of_three) { .small=baz, .middle=bar, .large=foo };
        }
    }
    return set;
}

u16 hash(struct set_of_three set) {
    __uint32_t h = 2166136261u;
    const __uint32_t prime = 16777619u;
    h = (h ^ set.small) * prime;
    h = (h ^ set.middle) * prime;
    h = (h ^ set.large) * prime;
    return h % HASH_SET_CAPACITY;
}

void set_add(struct three_sets *s, struct set_of_three addend) {
    u16 h = hash(addend);
    struct node *head = s->sets[h];
    struct node *node = head;
    u8 already_present = 0;
    while (node)
    {
        if (memcmp(&node->set, &addend, sizeof(struct set_of_three)) == 0)
        {
            already_present = 1;
            break;
        }
        node = node->next;
    }
    if (!already_present)
    {
        s->arena[s->size] = (struct node) { .set=addend, .next=head };
        s->sets[h] = &s->arena[s->size++];
    }
}

void main(int argc, char const *argv[])
{
    read_input();
    struct three_sets three_sets = {};
    // All pc's starting with 't'
    for (u16 i = ('t' - 'a') * ALPHABET; i < ('t' - 'a' + 1) * ALPHABET; i++)
    {
        if (!network.pcs_neighbors[i]) // PC doesn't exist in the network
        {
            continue;
        }
        for (u16 j = 0; j < GRAPH_SIZE; j++)
        {
            if (!(*network.pcs_neighbors[i])[j]) // PCs aren't connected
            {
                continue;
            }
            for (u16 k = 0; k < GRAPH_SIZE; k++)
            {
                if ((*network.pcs_neighbors[i])[k] & (*network.pcs_neighbors[j])[k]) {
                    struct set_of_three set = make_set(i, j, k);
                    set_add(&three_sets, set);
                }
            }   
        }
    }
    printf("t three set count: %hd\n", three_sets.size);
}
