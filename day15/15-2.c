#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define GRID_SIZE 50
#define QUEUE_POW_2_CONSTANT 4
#define QUEUE_SIZE (1 << QUEUE_POW_2_CONSTANT)
#define QUEUE_MASK (QUEUE_SIZE - 1)

typedef __uint8_t u8;
typedef __int8_t i8;
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

struct deque {
    struct robot ps[QUEUE_SIZE];
    u16 start;
    u16 end;
};

void try_to_move(i8 di, i8 dj, struct robot *r, char warehouse[GRID_SIZE][2 * GRID_SIZE]);
void try_to_move_large_box_horizontally(i8 dj, struct robot *r, char warehouse[GRID_SIZE][2 * GRID_SIZE]);
__uint8_t can_move_vertically(u8 i, u8 j, i8 di, char warehouse[GRID_SIZE][2 * GRID_SIZE]);
void move_large_box_vertically(u8 i, u8 left_j, i8 di, char warehouse[GRID_SIZE][2 * GRID_SIZE]);

u32 calculate_gps(char warehouse[GRID_SIZE][2 * GRID_SIZE]);
struct robot find_robot(char warehouse[GRID_SIZE][2 * GRID_SIZE]);

struct moves read_input(char warehouse[GRID_SIZE][2 * GRID_SIZE]);

void main(int argc, char const *argv[])
{
    char warehouse[GRID_SIZE][2 * GRID_SIZE];
    struct moves m = read_input(warehouse);
    struct robot r = find_robot(warehouse);
    while (m.size--)
    {
        i8 di, dj;
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

struct robot find_robot(char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
    for (u8 i = 0; i < GRID_SIZE; i++)
        for (u8 j = 0; j < 2 * GRID_SIZE; j++)
            if (warehouse[i][j] == '@')
                return (struct robot) { .i=i, .j=j };
}

void try_to_move(i8 di, i8 dj, struct robot *r, char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
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
    else if (dst == '[' || dst == ']')
    {
        if (dj){
            try_to_move_large_box_horizontally(dj, r, warehouse);
        } else if (can_move_vertically(dst_i, dst_j, di, warehouse))
        {
            __uint8_t offset = dst == ']';
            move_large_box_vertically(dst_i, dst_j - offset, di, warehouse);
            warehouse[dst_i][dst_j] = '@';
            warehouse[dst_i][dst_j + 1 - 2 * offset] = '.';
            warehouse[r->i][r->j] = '.';
            r->i = dst_i;
        }
    }
}

void try_to_move_large_box_horizontally(i8 dj, struct robot *r, char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
    u8 i = r->i;
    u8 dst_j = r->j + dj;

    for (;;)
    {
        char dst = warehouse[i][dst_j];
        if (dst == '#') {
            return;
        } else if (dst == '.') {
            break;
        }
        dst_j += 2 * dj;
    }

    u8 open_or_close = dj == 1 ? '[' : ']';
    u8 close_or_open = dj == 1 ? ']' : '[';

    for (u8 j = r->j + 2 * dj; dj < 0 ? j > dst_j : j < dst_j; j += 2 * dj)
    {
        warehouse[i][j] = open_or_close;
        warehouse[i][j + dj] = close_or_open;
    }
    
    warehouse[i][r->j] = '.';
    warehouse[i][r->j + dj] = '@';
    r->j += dj;
}

__uint8_t can_move_vertically(u8 i, u8 j, i8 di, char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
    char dst = warehouse[i][j];
    if (dst == '.') {
        return 1;
    } else if (dst == '#') {
        return 0;
    }
    u8 offset = dst == ']';
    i += di;
    return can_move_vertically(i, j - offset    , di, warehouse)
        && can_move_vertically(i, j - offset + 1, di, warehouse);
}

void enqueue(struct deque *q, struct robot p) {
    q->ps[q->end++] = p;
    q->end &= QUEUE_MASK;
}

struct robot dequeue(struct deque *q) {
    struct robot p = q->ps[q->start++];
    q->start &= QUEUE_MASK;
    return p;
}

struct robot pop(struct deque *q) {
    struct robot p = q->ps[--q->end];
    q->end &= QUEUE_MASK;
    return p;
}

void move_large_box_vertically(u8 i, u8 left_j, i8 di, char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
    struct deque acc = { .ps=(struct robot){ .i=i, .j=left_j }, .start=0, .end=1 };
    struct deque level = acc;

    while (level.start != level.end)
    {
        struct robot p = dequeue(&level);
        u8 next_i = p.i + di;
        char next_left = warehouse[next_i][p.j];
        if (next_left == '[') {
            struct robot box = { .i=next_i, .j=p.j };
            enqueue(&acc, box);
            enqueue(&level, box);
        } else {
            char next_right = warehouse[next_i][p.j + 1];
            if (next_left == ']') {
            struct robot box = { .i=next_i, .j=p.j - 1 };
            enqueue(&acc, box);
            enqueue(&level, box);
            }
            if (next_right == '[') {
            struct robot box = { .i=next_i, .j=p.j + 1 };
            enqueue(&acc, box);
            enqueue(&level, box);
            }
        }
    }
    
    while (acc.start != acc.end)
    {
        // Put [] at the destination. Put .. at the origin.
        struct robot p = pop(&acc);
        warehouse[p.i][p.j] = '.';
        warehouse[p.i][p.j + 1] = '.';
        warehouse[p.i + di][p.j] = '[';
        warehouse[p.i + di][p.j + 1] = ']';
    }
}

u32 calculate_gps(char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
    u32 gps = 0;
    for (u8 i = 0; i < GRID_SIZE; i++)
        for (u8 j = 0; j < 2 * GRID_SIZE; j++)
            gps += (warehouse[i][j] == '[') * (100 * i + j);
    return gps;
}

struct moves read_input(char warehouse[GRID_SIZE][2 * GRID_SIZE]) {
    FILE *f = fopen("15-input-warehouse", "r");
    char buffer[GRID_SIZE + 2];
    for (__uint8_t i = 0; i < GRID_SIZE; i++)
    {
        fgets(buffer, GRID_SIZE + 2, f);
        for (__uint8_t j = 0; j < GRID_SIZE; j++)
        {
            char first, second;
            switch (buffer[j])
            {
            case 'O': { first = '[', second = ']'; break; }
            case '@': { first = '@', second = '.'; break; }
            default : { first = buffer[j], second = buffer[j]; break; }
            }
            warehouse[i][2 * j] = first;
            warehouse[i][2 * j + 1] = second;
        }
    }
    fclose(f);
    
    int fd = open("15-input-moves", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    char *moves = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return (struct moves) { .next=moves, .size=sb.st_size };
}
