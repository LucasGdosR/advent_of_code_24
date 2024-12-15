#include <math.h>
#include <stdio.h>

#define ENTRIES 320
#define A_COST 3
#define B_COST 1
#define EPSILON 0.001
#define CORRECTION 10000000000000

typedef __int16_t i16;
typedef __int64_t i64;

i64 calc_cost(i16 ax, i16 ay, i16 bx, i16 by, i64 px, i64 py);
void read_input(i16 *ax, i16 *ay, i16 *bx, i16 *by, i64 *px, i64 *py);

void main(int argc, char const *argv[])
{
    i16 ax[ENTRIES], ay[ENTRIES], bx[ENTRIES], by[ENTRIES];
    i64 px[ENTRIES], py[ENTRIES];
    read_input(ax, ay, bx, by, px, py);

    i64 tokens = 0;
    for (short i = 0; i < ENTRIES; i++) {
        tokens += calc_cost(ax[i], ay[i], bx[i], by[i], px[i] + CORRECTION, py[i] + CORRECTION);
    }
    printf("Tokens necessary: %ld\n", tokens);
}

i64 calc_cost(i16 ax, i16 ay, i16 bx, i16 by, i64 px, i64 py) {
    double denominator = ax*by - bx*ay;
    double a_pressed = (px*by - bx*py) / denominator;
    double b_pressed = (ax*py - ay*px) / denominator;
    i16 impossible = (fabs(a_pressed - round(a_pressed)) > EPSILON)
                 || (fabs(b_pressed - round(b_pressed)) > EPSILON);
    return impossible ? 0 : (round(a_pressed) * A_COST) + (round(b_pressed) * B_COST);
}

void read_input(i16 *ax, i16 *ay, i16 *bx, i16 *by, i64 *px, i64 *py) {
    FILE *fptr = fopen("13-input", "r");
    char buffer[32];
    for(short i = 0; i < ENTRIES; i++) {
        fgets(buffer, sizeof(buffer), fptr);
        sscanf(buffer, "Button A: X+%hd, Y+%hd", &ax[i], &ay[i]);
        fgets(buffer, sizeof(buffer), fptr);
        sscanf(buffer, "Button B: X+%hd, Y+%hd", &bx[i], &by[i]);
        fgets(buffer, sizeof(buffer), fptr);
        sscanf(buffer, "Prize: X=%ld, Y=%ld", &px[i], &py[i]);        
        fgets(buffer, sizeof(buffer), fptr);
    }
    fclose(fptr);
}
