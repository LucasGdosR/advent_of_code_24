#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

enum State {
    NONE,
    M,
    MU,
    MUL,
    MULp,
    MULpd,
    MULpdd,
    MULpddd,
    MULpd1_3c,
    MULpd1_3cd,
    MULpd1_3cdd,
    MULpd1_3cddd,
    D,
    DO,
    DOp,
    DON,
    DONq,
    DONqT,
    DONqTp,
};

#define DEFAULT_TRANSITION curr == 'm' ? M : curr == 'd' ? D : NONE;

void main(int argc, char *argv[]) {
    int fd = open("3-input", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    char *input = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    int sum = 0;
    int mulA = 0;
    int mulB = 0;
    int state = 0;
    int du = 1;
    for (int i = 0; i < sb.st_size; i++) {
        char curr = input[i];
        switch (state) {
        case NONE:
            state = DEFAULT_TRANSITION
            break;
        case M:
            state = curr == 'u' ? MU
                  : DEFAULT_TRANSITION
            break;
        case MU:
            state = curr == 'l' ? MUL
                  : DEFAULT_TRANSITION
            break;
        case MUL:
            state = curr == '(' ? MULp
                  : DEFAULT_TRANSITION
            break;
        case MULp:
            if ('0' <= curr && curr <= '9') {
                state = MULpd;
                mulA = curr - '0';
            } else {
                state = DEFAULT_TRANSITION
            }
            break;
        case MULpd:
        case MULpdd:
            if ('0' <= curr && curr <= '9') {
                state++;
                mulA = mulA * 10 + curr - '0';
            } else if (curr == ',') {
                state = MULpd1_3c;
            } else {
                state = DEFAULT_TRANSITION
                mulA = 0;
            }
            break;
        case MULpddd:
            if (curr == ',') {
                state = MULpd1_3c;
            } else {
                state = DEFAULT_TRANSITION
                mulA = 0;
            }
            break;
        case MULpd1_3c:
            if ('0' <= curr && curr <= '9') {
                state = MULpd1_3cd;
                mulB = curr - '0';
            } else {
                state = DEFAULT_TRANSITION
                mulA = 0;
            }
            break;
        case MULpd1_3cd:
        case MULpd1_3cdd:
            if ('0' <= curr && curr <= '9') {
                state++;
                mulB = mulB * 10 + curr - '0';
            } else {
                sum = sum + (du && (curr == ')') ? mulA * mulB : 0);
                state = DEFAULT_TRANSITION
                mulA = 0;
                mulB = 0;
            }
            break;
        case MULpd1_3cddd:
            sum = sum + (du && curr == ')' ? mulA * mulB : 0);
            state = DEFAULT_TRANSITION
            mulA = 0;
            mulB = 0;
            break;
        case D:
            state = curr == 'o' ? DO
                  : DEFAULT_TRANSITION
            break;
        case DO:
            state = curr == '(' ? DOp
                  : curr == 'n' ? DON
                  : DEFAULT_TRANSITION
            break;
        case DOp:
            du = curr == ')' ? M : du;
            state = DEFAULT_TRANSITION
            break;
        case DON:
            state = curr == '\'' ? DONq
                  : DEFAULT_TRANSITION
            break;
        case DONq:
            state = curr == 't' ? DONqT
                  : DEFAULT_TRANSITION      
            break;  
        case DONqT:
            state = curr == '(' ? DONqTp
                  : DEFAULT_TRANSITION        
            break;
        case DONqTp:
            state = DEFAULT_TRANSITION
            du = curr == ')' ? 0 : du;
            break;
        default:
            state = NONE;
            mulA = 0;
            mulB = 0;
            break;
        }
    }
    printf("Sum of products: %d\n", sum);
}
