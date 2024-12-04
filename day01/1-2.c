#include <stdio.h>

#define LINE_LENGTH 16
#define INPUT_SIZE 1000
#define HASHTABLE_SIZE 2 * INPUT_SIZE

struct node {
    int key;
    int value;
    struct node *next;
};

struct arena {
    struct node nodes[INPUT_SIZE];
    int arena_alloc_pos;
};

struct node NIL_NODE = { .key = 0, .value = 0, .next = &NIL_NODE };

struct node* update_list(struct node *head, struct node *next, int key, struct arena *arena);
int get_similarity(struct node *node, int key);

void main(int argc, char const *argv[]) {
    FILE *fptr = fopen("1-input", "r");
    char line[LINE_LENGTH];
    int fst_list[INPUT_SIZE];
    
    struct node *snd_list_counter[HASHTABLE_SIZE];
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        snd_list_counter[i] = &NIL_NODE;
    }
    
    struct arena arena;
    arena.arena_alloc_pos = 0;

    int i = 0;
    while(fgets(line, LINE_LENGTH, fptr)) {
        int j;
        sscanf(line, "%d   %d", &fst_list[i], &j);
        struct node *head = snd_list_counter[j % HASHTABLE_SIZE];
        snd_list_counter[j % HASHTABLE_SIZE] = update_list(head, head, j, &arena);
        i++;
    }
    fclose(fptr);
    
    int similarity = 0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        int e = fst_list[i];
        similarity += e * get_similarity(snd_list_counter[e % HASHTABLE_SIZE], e);
    }
    printf("Similarity: %d\n", similarity);
}

struct node* update_list(struct node *head, struct node *next, int key, struct arena *arena) {
    if (next == &NIL_NODE) {
        arena->nodes[arena->arena_alloc_pos] = (struct node) { .key = key, .value = 1, .next = head };
        return &arena->nodes[arena->arena_alloc_pos++];
    } else if (next->key == key) {
        next->value++;
        return head;
    } else {
        return update_list(head, next->next, key, arena);
    }
}

int get_similarity(struct node *node, int key) {
    if (node->key == key || node == &NIL_NODE) {
        return node->value;
    } else {
        return get_similarity(node->next, key);
    }
}
