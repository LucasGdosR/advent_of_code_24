#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 3380
#define LINE_LENGTH 7
#define GRAPH_SIZE ('z' - 'a' + 1)
#define HASH_SET_SIZE 1230
#define HASH_SET_CAPACITY 1657 // prime > size / 0.75

typedef unsigned char u8;

struct pc { u8 a, b; };

struct set_of_three { struct pc small, middle, large; };

struct node {
    struct set_of_three set;
    struct node *next;
};

struct three_sets {
    struct node *sets[HASH_SET_CAPACITY];
    struct node arena[HASH_SET_SIZE];
    short size;
};

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

// this is smaller: -1
// that is smaller:  1
signed char compare_pc(struct pc this, struct pc that) {
    return (((short)this.a) * GRAPH_SIZE + this.b) < (((short)that.a) * GRAPH_SIZE + that.b) ? -1 : 1;
}

struct set_of_three make_set(struct pc foo, struct pc bar, struct pc baz) {
    struct set_of_three set;
    if (compare_pc(foo, bar) < 0){
        if (compare_pc(foo, baz) < 0){
            if (compare_pc(bar, baz) < 0) {
                set = (struct set_of_three) { .small=foo, .middle=bar, .large=baz };
            } else {
                set = (struct set_of_three) { .small=foo, .middle=baz, .large=bar };
            }
        } else {
            set = (struct set_of_three) { .small=baz, .middle=foo, .large=bar };
        }
    } else {
        if (compare_pc(bar, baz) < 0) {
            if (compare_pc(foo, baz) < 0) {
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

short hash(struct set_of_three set) {
    const size_t prime1 = 31;
    const size_t prime2 = 37;
    
    size_t hash = set.small.a;
    hash = hash * prime1 + set.small.b;
    hash = hash * prime2 + set.middle.a;
    hash = hash * prime2 + set.middle.b;
    hash = hash * prime1 + set.large.a;
    hash = hash * prime1 + set.large.b;
    
    return hash % HASH_SET_CAPACITY;
}

void set_add(struct three_sets *s, struct set_of_three addend) {
    short h = hash(addend);
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
    if (s->size > HASH_SET_SIZE) printf("Arena overflowed\n");
}

void main(int argc, char const *argv[])
{
    read_input();
    struct three_sets three_sets = {};
    // All pc's starting with 't'
    for (u8 i = 0; i < GRAPH_SIZE; i++)
    {
        // First char of second pc
        for (u8 j = 0; j < GRAPH_SIZE; j++)
        {
            // Second char of second pc
            for (u8 k = 0; k < GRAPH_SIZE; k++)
            {
                if ((!adj_lists['t' - 'a'][i][j][k]) || ((j == ('t' - 'a')) && (i == k)))
                    continue;
                // First char of third pc
                for (u8 m = 0; m < GRAPH_SIZE; m++)
                {
                    // Second char of third pc
                    for (u8 n = 0; n < GRAPH_SIZE; n++)
                    {
                        if (adj_lists['t' - 'a'][i][m][n] & adj_lists[j][k][m][n]) {
                            struct pc foo = { .a=('t'-'a'), .b=i }, bar = { .a=j, .b=k }, baz = { .a=m, .b=n };
                            struct set_of_three set = make_set(foo, bar, baz);
                            set_add(&three_sets, set);
                        }
                    }
                }
            }   
        }
    }
    printf("t three set count: %hd\n", three_sets.size);
}
