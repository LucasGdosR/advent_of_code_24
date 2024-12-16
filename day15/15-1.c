#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define GRID_SIZE 50

typedef __uint8_t u8;
typedef __uint16_t u16;
typedef __uint32_t u32;

struct robot {
    u8 i;
    u8 j;
};

struct moves {
    char *next;
    u16 size;
};

void try_to_move(u8 di, u8 dj, struct robot *r, char warehouse[GRID_SIZE][GRID_SIZE + 2]);
u32 calculate_gps(char warehouse[GRID_SIZE][GRID_SIZE + 2]);
struct robot find_robot(char warehouse[GRID_SIZE][GRID_SIZE + 2]);
struct moves read_input(char warehouse[GRID_SIZE][GRID_SIZE + 2]);

void main(int argc, char const *argv[])
{
    char warehouse[GRID_SIZE][GRID_SIZE + 2];
    struct moves m = read_input(warehouse);
    struct robot r = find_robot(warehouse);
    while (m.size--)
    {
        u8 di, dj;
        switch (*m.next++)
        {
        case '<': { di =  0, dj = -1; break; }
        case '>': { di =  0, dj =  1; break; }
        case '^': { di = -1, dj =  0; break; }
        case 'v': { di =  1, dj =  0; break; }
        default:  { di =  0, dj =  0; break; }
        }
        try_to_move(di, dj, &r, warehouse);
    }
    printf("GPS: %d\n", calculate_gps(warehouse));
}

struct robot find_robot(char warehouse[GRID_SIZE][GRID_SIZE + 2]) {
    for (u8 i = 0; i < GRID_SIZE; i++)
        for (u8 j = 0; j < GRID_SIZE; j++)
            if (warehouse[i][j] == '@')
                return (struct robot) { .i=i, .j=j };
}

void try_to_move(u8 di, u8 dj, struct robot *r, char warehouse[GRID_SIZE][GRID_SIZE + 2]) {
    u8 dst_i = r->i + di;
    u8 dst_j = r->j + dj;
    char dst = warehouse[dst_i][dst_j];
    if (dst == '.')
    {
        warehouse[r->i][r->j] = '.';
        warehouse[dst_i][dst_j] = '@';
        r->i += di;
        r->j += dj;
    }
    else if (dst == 'O')
    {
        for (;;)
        {
            dst_i += di;
            dst_j += dj;
            dst = warehouse[dst_i][dst_j];
            if (dst == '#') {
                return;
            }
            else if (dst == '.')
            {
                warehouse[dst_i][dst_j] = 'O';
                break;
            }
        }    
        warehouse[r->i][r->j] = '.';
        r->i += di;
        r->j += dj;
        warehouse[r->i][r->j] = '@';
    }
}

u32 calculate_gps(char warehouse[GRID_SIZE][GRID_SIZE + 2]) {
    u32 gps = 0;
    for (u8 i = 0; i < GRID_SIZE; i++)
        for (u8 j = 0; j < GRID_SIZE; j++)
            gps += (warehouse[i][j] == 'O') * (100 * i + j);
    return gps;
}

struct moves read_input(char warehouse[GRID_SIZE][GRID_SIZE + 2]) {
    FILE *f = fopen("15-input-warehouse", "r");
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        fgets(warehouse[i], GRID_SIZE + 2, f);
    }
    fclose(f);
    
    int fd = open("15-input-moves", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    char *moves = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return (struct moves) { .next=moves, .size=sb.st_size };
}
