#include <stdio.h>
#include <stdlib.h>

#define MAX_LEVELS_PER_REPORT 8
#define LINE_LENGTH 32
#define INPUT_SIZE 1000

void read_input(char *buffer, char *file_path, long int output[][MAX_LEVELS_PER_REPORT]);
int is_safe(long int *report);

void main(int argc, char const *argv[]) {
    char line[LINE_LENGTH];
    long int reports[INPUT_SIZE][MAX_LEVELS_PER_REPORT] = {};
    read_input(line, "2-input", reports);

    int safe_reports = 0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        safe_reports += is_safe(reports[i]);
    }
    printf("Safe reports: %d\n", safe_reports);
}

int is_safe(long int *report) {
    int is_increasing = report[0] < report[1];
    for (int i = 1; i < MAX_LEVELS_PER_REPORT && report[i]; i++) {
        long prev = report[i - 1], curr = report[i];
        if (prev < curr != is_increasing ||
            prev == curr ||
            abs(prev - curr) > 3) {
            return 0;
        }
    }
    return 1;
}

void read_input(char *buffer, char *file_path, long int output[][MAX_LEVELS_PER_REPORT]) {
    FILE *fptr = fopen(file_path, "r");
    short i = 0;
    while(fgets(buffer, LINE_LENGTH, fptr)) {
        char *token;
        char *rest = buffer;
        short j = 0;
        while (token = strsep(&rest, " ")) {
            char *ep;
            output[i][j] = strtol(token, &ep, 10);
            j++;
        }
        i++;
    }
    fclose(fptr);
}
