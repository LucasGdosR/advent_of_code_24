#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 500
#define WIDTH 101
#define HEIGHT 103
#define FOUR_DOTS 0x2E2E2E2E

struct robot {
    int px;
    int py;
    int vx;
    int vy;
};

void look_for_tree(int simulation_time, struct robot *robots) {
    char viz[HEIGHT][WIDTH + 2]; // +2 for '\n' and '\0'
    memset(viz, FOUR_DOTS, sizeof(viz));
    for (int i = 0; i < INPUT_SIZE; i++)
    {
        struct robot r = robots[i];
        int x = (r.px + r.vx * simulation_time) % WIDTH;
        x += (x < 0) * WIDTH;
        int y = (r.py + r.vy * simulation_time) % HEIGHT;
        y += (y < 0) * HEIGHT;
        viz[y][x] = '#';
    }
    printf("__________________________\nTIME: %d\n__________________________\n", simulation_time);
    for (int i = 0; i < HEIGHT; i++)
    {
        viz[i][WIDTH] = '\n';
        viz[i][WIDTH + 1] = '\0';
        printf("%s", viz[i]);
    }
}

void read_input(struct robot *r);

void main(int argc, char const *argv[])
{
    struct robot robots[INPUT_SIZE];
    read_input(robots);
    // Check the Python file for an explanation on how to find your value
    // using prime number math and visual inspection.
    look_for_tree(7383, robots);
}

void read_input(struct robot *r) {
    FILE *f = fopen("14-input", "r");
    char buffer[32];
    for (int i = 0; i < INPUT_SIZE; i++) {
        fgets(buffer, sizeof(buffer), f);
        sscanf(buffer, "p=%d,%d v=%d,%d", &r[i].px, &r[i].py, &r[i].vx, &r[i].vy);
    }
    fclose(f);
}
