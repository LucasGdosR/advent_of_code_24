#include <stdio.h>

#define INPUT_SIZE 1506
#define BUFFER_SIZE 10

void read_input(int seeds[INPUT_SIZE]);

void main(int argc, char const *argv[])
{
    int seeds[INPUT_SIZE];
    read_input(seeds);
    long total = 0;
    for (short i = 0; i < INPUT_SIZE; i++)
    {
        int seed = seeds[i];
        for (size_t i = 0; i < 2000; i++)
        {
            seed = (seed ^ (seed << 6)) & 0xFFFFFF;
            seed = (seed ^ (seed >> 5)) & 0xFFFFFF;
            seed = (seed ^ (seed << 11)) & 0xFFFFFF;
        }
        total += seed;
    }
    printf("Sum of 2000th secret numbers: %ld\n", total);
}

void read_input(int seeds[INPUT_SIZE]) {
    FILE *f = fopen("22-input", "r");
    char buffer[BUFFER_SIZE];
    for (size_t i = 0; i < INPUT_SIZE; i++)
    {
        fgets(buffer, BUFFER_SIZE, f);
        sscanf(buffer, "%d", &seeds[i]);
    }
    fclose(f);
}
