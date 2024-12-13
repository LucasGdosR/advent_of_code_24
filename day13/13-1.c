#include <math.h>
#include <stdio.h>

#define ENTRIES 320
#define A_COST 3
#define B_COST 1
#define EPSILON 0.001

typedef __uint8_t u8;
typedef __uint64_t u64;

u64 calc_cost(u8 ax, u8 ay, u8 bx, u8 by, u64 px, u64 py);
void read_input(u8 *ax, u8 *ay, u8 *bx, u8 *by, u64 *px, u64 *py);

void main(int argc, char const *argv[])
{
    u8 ax[ENTRIES], ay[ENTRIES], bx[ENTRIES], by[ENTRIES];
    u64 px[ENTRIES], py[ENTRIES];
    read_input(ax, ay, bx, by, px, py);

    u64 tokens = 0;
    for (short i = 0; i < ENTRIES; i++) {
        tokens += calc_cost(ax[i], ay[i], bx[i], by[i], px[i], py[i]);
    }
    printf("Tokens necessary: %ld\n", tokens);
}

u64 calc_cost(u8 ax, u8 ay, u8 bx, u8 by, u64 px, u64 py) {
    double denominator = ax*by - bx*ay;
    double a_pressed = (px*by - bx*py) / denominator;
    double b_pressed = (ax*py - ay*px) / denominator;
    u8 impossible = (fabs(a_pressed - round(a_pressed)) > EPSILON)
                 || (fabs(b_pressed - round(b_pressed)) > EPSILON);
    return impossible ? 0 : (round(a_pressed) * A_COST) + (round(b_pressed) * B_COST);
}

void read_input(u8 *ax, u8 *ay, u8 *bx, u8 *by, u64 *px, u64 *py) {
    FILE *fptr = fopen("13-input", "r");
    char buffer[32];
    for(short i = 0; i < ENTRIES; i++) {
        fgets(buffer, 32, fptr);
        sscanf(buffer, "Button A: X+%d, Y+%d", &ax[i], &ay[i]);
        fgets(buffer, 32, fptr);
        sscanf(buffer, "Button B: X+%d, Y+%d", &bx[i], &by[i]);
        fgets(buffer, 32, fptr);
        sscanf(buffer, "Prize: X+%d, Y+%d", &px[i], &py[i]);
        if (i != ENTRIES -1) {
            fgets(buffer, 32, fptr);
        }
    }
}
