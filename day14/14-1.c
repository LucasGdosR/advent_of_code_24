#include <stdio.h>

#define INPUT_SIZE 500
#define SIMULATION_TIME 100
#define WIDTH 101
#define HEIGHT 103
#define DELIM_X (WIDTH >> 1)
#define DELIM_Y (HEIGHT >> 1)

void main(int argc, char const *argv[])
{
    FILE *f = fopen("14-input", "r");
    char buffer[32];
    int robots_per_quadrant[4] = {};
    for (int i = 0, px, py, vx, vy; i < INPUT_SIZE; i++)
    {
        fgets(buffer, sizeof(buffer), f);
        sscanf(buffer, "p=%d,%d v=%d,%d", &px, &py, &vx, &vy);
        int final_x = (px + vx * SIMULATION_TIME) % WIDTH;
        final_x += (final_x < 0) * WIDTH;
        int final_y = (py + vy * SIMULATION_TIME) % HEIGHT;
        final_y += (final_y < 0) * HEIGHT;
        robots_per_quadrant[0] += final_x < DELIM_X && final_y < DELIM_Y;
        robots_per_quadrant[1] += final_x > DELIM_X && final_y < DELIM_Y;
        robots_per_quadrant[2] += final_x > DELIM_X && final_y > DELIM_Y;
        robots_per_quadrant[3] += final_x < DELIM_X && final_y > DELIM_Y;
    }
    fclose(f);
    printf("Safety factor: %d\n",
        robots_per_quadrant[0] *
        robots_per_quadrant[1] *
        robots_per_quadrant[2] *
        robots_per_quadrant[3]
    );
}
