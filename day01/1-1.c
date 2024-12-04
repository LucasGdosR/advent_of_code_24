#include <stdio.h>
#include "sort.c"

#define LINE_LENGTH 16
#define INPUT_SIZE 1000

void main(int argc, char const *argv[]) {
    FILE *fptr = fopen("1-input", "r");
    char line[LINE_LENGTH];
    int fst_list[INPUT_SIZE];
    int snd_list[INPUT_SIZE];

    short i = 0;
    while(fgets(line, LINE_LENGTH, fptr)) {
        sscanf(line, "%d   %d", &fst_list[i], &snd_list[i]);
        i++;
    }
    fclose(fptr);
    
    sort(fst_list, INPUT_SIZE);
    sort(snd_list, INPUT_SIZE);

    int distance = 0;
    for (short i = 0; i < INPUT_SIZE; i++) {
        distance += abs(fst_list[i] - snd_list[i]);
    }
    printf("Distance: %d\n", distance);
}
