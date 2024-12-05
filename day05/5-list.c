#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAGES_PER_UPDATE 23
#define LINE_LENGTH 70
#define UPPER_BOUND_PAGE 100
#define PAGE_ORDERING_RULES 1176
#define INPUT_SIZE 1366
#define UPDATES INPUT_SIZE - PAGE_ORDERING_RULES - 1

struct node {
    int val;
    struct node *next;
};

struct adj_list {
    struct node *adj_pages[UPPER_BOUND_PAGE];
    struct node arena[PAGE_ORDERING_RULES];
};

int is_valid(int update[MAX_PAGES_PER_UPDATE]);
int fix(int update[MAX_PAGES_PER_UPDATE]);
void read_input();
void init_adj_list(FILE *fp, char buffer[LINE_LENGTH]);
void init_updates(FILE *fp, char buffer[LINE_LENGTH]);

struct node NIL_NODE = { .val = 0, .next = &NIL_NODE };
struct adj_list adj_list;
int updates[UPDATES][MAX_PAGES_PER_UPDATE];

void main(int argc, char const *argv[]) {
    read_input();
    
    int valid_sum = 0, incorrect_sum = 0;
    for (int i = 0; i < UPDATES; i++) {
        int *update = updates[i];
        int valid_update = is_valid(update);
        if (valid_update) {
            valid_sum += update[valid_update];
        } else {
            incorrect_sum += fix(update);
        }
    }
    printf("Sum of valid middle pages: %d\n\
Sum of fixed middle pages: %d\n", valid_sum, incorrect_sum);
}

int is_valid(int update[MAX_PAGES_PER_UPDATE]) {
    int i;
    for (i = 0; i < MAX_PAGES_PER_UPDATE && update[i]; i++) {
        int page = update[i];
        struct node *prereqs = adj_list.adj_pages[page];
        while (prereqs != &NIL_NODE) {
            int prereq = prereqs->val;
            prereqs = prereqs->next;

            for (int j = 0; j < MAX_PAGES_PER_UPDATE; j++) {
                if (prereq == update[j]) {
                    if (j > i)
                        return 0;
                    else break;
                }
            }
        }
    }
    return i / 2;
}

int fix(int update[MAX_PAGES_PER_UPDATE]) {
    int len = 0, correct_order[MAX_PAGES_PER_UPDATE];
    memset(correct_order, 0, sizeof(correct_order));

    while (update[len] && len < MAX_PAGES_PER_UPDATE) {
        // Iterate through pages in the update
        for (int i = 0; i < MAX_PAGES_PER_UPDATE && update[i]; i++) {
            int page = update[i];
            
            // Check if page has already been added
            int page_already_added = 0;
            for (int i = 0; i < len; i++) {
                if (correct_order[i] == page)
                    page_already_added = 1;
            }
            if (page_already_added) {
                continue;
            }
            
            // Check if there are still prereqs to be met
            int unmet_prereqs = 0;
            struct node *prereqs = adj_list.adj_pages[page];
            // Iterate through prereqs in adjacency list
            while (prereqs != &NIL_NODE) {
                int prereq = prereqs->val;
                prereqs = prereqs->next;

                // If all prereqs present in update have already been added,
                // increments and decrements will cancel out
                for (int j = 0; j < MAX_PAGES_PER_UPDATE; j++) {
                    if (prereq == update[j]) {
                        unmet_prereqs++;
                    }
                    if (prereq == correct_order[j]) {
                        unmet_prereqs--;
                    }
                }       
            }
            
            if (!unmet_prereqs) {
                correct_order[len++] = page;
            }
        }
    }
    return correct_order[len / 2];
}

void read_input() {
    FILE *fp = fopen("5-input", "r");
    char buffer[LINE_LENGTH];
    init_adj_list(fp, buffer);
    fgets(buffer, LINE_LENGTH, fp); // Skip blank line
    init_updates(fp, buffer);
    fclose(fp);
}

void init_adj_list(FILE *fp, char *buffer) {
    for (int i = 0; i < UPPER_BOUND_PAGE; i++) {
        adj_list.adj_pages[i] = &NIL_NODE;
    }
    
    for (int i = 0, src, dst; i < PAGE_ORDERING_RULES; i++) {
        fgets(buffer, LINE_LENGTH, fp);
        sscanf(buffer, "%d|%d", &dst, &src);
        adj_list.arena[i] = (struct node) { .next = adj_list.adj_pages[src], .val = dst };
        adj_list.adj_pages[src] = &adj_list.arena[i];
    }
}

void init_updates(FILE *fp, char *buffer) {
    memset(updates, 0, sizeof(updates));
    
    for (int i = 0; i < UPDATES; i++) {
        fgets(buffer, LINE_LENGTH, fp);
        char *token, *rest = buffer;
        int j = 0;
        while (token = strsep(&rest, ",")) {
            char *ep;
            updates[i][j++] = strtol(token, &ep, 10);
        }
    }
}
