#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVELS_PER_REPORT 8
#define LINE_LENGTH 32
#define INPUT_SIZE 1000

void read_input(char *buffer, char *file_path, long int output[][MAX_LEVELS_PER_REPORT]);
int is_safe(long int *report);
void generate_dampened_reports(long int *report, long int buffer[][MAX_LEVELS_PER_REPORT]);

void main(int argc, char const *argv[]) {
    char line[LINE_LENGTH];
    long int reports[INPUT_SIZE][MAX_LEVELS_PER_REPORT];
    memset(reports, 0, sizeof(reports));
    read_input(line, "2-input", reports);
    
    int safe_reports = 0;
    for (int i = 0; i < INPUT_SIZE; i++) {
        long int dampened_reports[MAX_LEVELS_PER_REPORT][MAX_LEVELS_PER_REPORT];
        for (int i = 0; i < MAX_LEVELS_PER_REPORT; i++) 
            for (int j = 0; j < MAX_LEVELS_PER_REPORT; j++)
                dampened_reports[i][j] = 0;
        generate_dampened_reports(reports[i], dampened_reports);
        for (int i = 0; i < MAX_LEVELS_PER_REPORT; i++) {
            if (is_safe(dampened_reports[i])) {
                safe_reports++;
                break;
            }
        }
    }
    printf("Safe reports: %d\n", safe_reports);
}

void generate_dampened_reports(long int *report, long int buffer[][MAX_LEVELS_PER_REPORT]) {
    for (int i = 0; i < MAX_LEVELS_PER_REPORT; i++)
        for (int j = 0; j < MAX_LEVELS_PER_REPORT; j++)
            if (i != j)
                buffer[j][i - (j < i)] = report[i];
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
