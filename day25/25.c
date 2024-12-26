#include <stdio.h>

#define LOCK_QTY 500
#define LOCK_HEIGHT 7
#define LINE_LENGTH 7
#define MAX_SUM 25
#define COLS 5

struct node {
    char key_or_lock[COLS];
    struct node *next;
};

struct dict {
    struct node *by_sum[MAX_SUM + 1];
    struct node arena[LOCK_QTY];
    short size;
};

struct node compress_string_to_arr(char key_or_lock[LOCK_HEIGHT][LINE_LENGTH]);
char check_overlap(struct node *k, struct node *l);

void read_input(char keys_n_locks[LOCK_QTY][LOCK_HEIGHT][LINE_LENGTH]);

void main(int argc, char const *argv[])
{
    char keys_n_locks[LOCK_QTY][LOCK_HEIGHT][LINE_LENGTH];
    read_input(keys_n_locks);

    struct dict keys, locks;
    for (short i = 0; i < LOCK_QTY; i++)
    {
        struct node n = compress_string_to_arr(keys_n_locks[i]);
        char sum = 0;
        for (char i = 0; i < COLS; i++)
        {
            sum += n.key_or_lock[i];
        }
        if (keys_n_locks[i][0][0] == '#')
        {
            n.next = locks.by_sum[sum];
            locks.arena[locks.size] = n;
            locks.by_sum[sum] = &locks.arena[locks.size++];
        }
        else
        {
            n.next = keys.by_sum[sum];
            keys.arena[keys.size] = n;
            keys.by_sum[sum] = &keys.arena[keys.size++];
        }
    }
    
    short possible_matches = 0;
    for (char i = 0; i <= MAX_SUM; i++)
    {
        struct node *k = keys.by_sum[i];
        while (k)
        {
            for (char i = 0; i <= MAX_SUM; i++)
            {
                struct node *l = locks.by_sum[i];
                while (l)
                {
                    possible_matches += check_overlap(k->key_or_lock, l->key_or_lock);
                    l = l->next;
                }
            }
            k = k->next;
        }   
    } 
}

struct node compress_string_to_arr(char str_repr[LOCK_HEIGHT][LINE_LENGTH]) {
    struct node int_repr = {};
    for (char i = 1; i < LOCK_HEIGHT - 1; i++)
    {
        int_repr.key_or_lock[0] += str_repr[i][0] == '#';
        int_repr.key_or_lock[1] += str_repr[i][1] == '#';
        int_repr.key_or_lock[2] += str_repr[i][2] == '#';
        int_repr.key_or_lock[3] += str_repr[i][3] == '#';
        int_repr.key_or_lock[4] += str_repr[i][4] == '#';
    }
    return int_repr;
}

char check_overlap(char k[COLS], char l[COLS]) {
    char no_overlap = 1;

    for (char i = 0; i < COLS; i++)
    {
        no_overlap &= (k[i] + l[i]) <= 5;
    }

    return no_overlap;
}

void read_input(char keys_n_locks[LOCK_QTY][LOCK_HEIGHT][LINE_LENGTH]) {
    FILE *f = fopen("25-input", "r");
    char _[2];
    for (short i = 0; i < LOCK_QTY; i++)
    {
        for (char j = 0; j < LOCK_HEIGHT; j++)
        {
            fgets(keys_n_locks[i][j], LINE_LENGTH, f);
        }
        fgets(_, 2, f); // skip blank line
    }
    fclose(f);
}
