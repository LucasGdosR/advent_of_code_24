#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define NUMBER_OF_FILES 10000
#define NUMBER_OF_SPACES 9999
#define INPUT_SIZE (NUMBER_OF_FILES + NUMBER_OF_SPACES)
#define POSSIBLE_SIZES_UPPER_BOUND 10
#define ZERO ((__uint64_t) '0')
#define CHAR_TO_NUM(c) (c - ((ZERO << 56) | (ZERO << 48) | (ZERO << 40) | (ZERO << 32) | (ZERO << 24) | (ZERO << 16) | (ZERO << 8) | ZERO))
#define GET_NTH_INPUT(input, n) (input  >> (8 * n))

__uint64_t *read_input();
void init_global_arrays(__uint64_t *input);
void defrag_file_system();
__uint64_t checksum();
void print_arrays();

__uint32_t file_ptrs[NUMBER_OF_FILES];
__uint8_t file_sizes[NUMBER_OF_FILES];
__uint32_t space_ptrs[NUMBER_OF_SPACES];
__uint8_t space_sizes[NUMBER_OF_SPACES];
__uint16_t search_caches[POSSIBLE_SIZES_UPPER_BOUND];

void main(int argc, char *argv[]) {
    __uint64_t *input = read_input();
    init_global_arrays(input);
    defrag_file_system();
    printf("Checksum: %ld\n", checksum());
}

void defrag_file_system() {
    for (__int16_t fid = NUMBER_OF_FILES - 1; fid >= 0; fid--)
    {
        __uint32_t fptr = file_ptrs[fid];
        __uint8_t fsize = file_sizes[fid];
        for (__uint16_t sid = search_caches[fsize]; sid < NUMBER_OF_SPACES; sid++)
        {
            __uint32_t sptr = space_ptrs[sid];
            if (fptr < sptr)
            {
                for (__uint8_t i = fsize; i < POSSIBLE_SIZES_UPPER_BOUND; i++)
                {
                    search_caches[i] = NUMBER_OF_SPACES;
                }
                break;
            }
            __uint8_t ssize = space_sizes[sid];
            if (fsize <= ssize) {
                file_ptrs[fid] = sptr;
                space_ptrs[sid] += fsize;
                space_sizes[sid] -= fsize;
                for (__uint8_t i = fsize; i < POSSIBLE_SIZES_UPPER_BOUND; i++)
                {
                    __uint16_t curr = search_caches[i];
                    search_caches[i] = sid > curr ? sid : curr;
                }
                break;
            }
        }   
    }
}

__uint64_t checksum() {
    __uint64_t sum = 0;
    for (__uint16_t i = 0; i < NUMBER_OF_FILES; i++)
    {
        __uint8_t fsize = file_sizes[i];
        __uint64_t fptr = (__uint64_t) file_ptrs[i];
        sum += i * ((fptr << 1) + fsize - 1) * fsize >> 1;
    }
    return sum;
}

__uint64_t *read_input() {
    int fd = open("9-input", O_RDONLY);
    __uint64_t *input = mmap(NULL, INPUT_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return input;
}

void init_global_arrays(__uint64_t *input) {
    __uint32_t fs_ptr = 0;
    __uint32_t *local_file_ptrs = file_ptrs;
    __uint32_t *local_space_ptrs = space_ptrs;
    __uint8_t *local_file_sizes = file_sizes;
    __uint8_t *local_space_sizes = space_sizes;
    __uint16_t count = INPUT_SIZE >> 3;
    
    while (count--)
    {
        __uint64_t eight_inputs = CHAR_TO_NUM(*(input++));

        local_file_ptrs[0] = fs_ptr;
        __uint8_t f1 = eight_inputs;
        fs_ptr += f1;
        local_file_sizes[0] = f1;

        local_space_ptrs[0] = fs_ptr;
        __uint8_t s1 = GET_NTH_INPUT(eight_inputs, 1);
        fs_ptr += s1;
        local_space_sizes[0] = s1;

        local_file_ptrs[1] = fs_ptr;
        __uint8_t f2 = GET_NTH_INPUT(eight_inputs, 2);
        fs_ptr += f2;
        local_file_sizes[1] = f2;

        local_space_ptrs[1] = fs_ptr;
        __uint8_t s2 = GET_NTH_INPUT(eight_inputs, 3);
        fs_ptr += s2;
        local_space_sizes[1] = s2;

        local_file_ptrs[2] = fs_ptr;
        __uint8_t f3 = GET_NTH_INPUT(eight_inputs, 4);
        fs_ptr += f3;
        local_file_sizes[2] = f3;

        local_space_ptrs[2] = fs_ptr;
        __uint8_t s3 = GET_NTH_INPUT(eight_inputs, 5);
        fs_ptr += s3;
        local_space_sizes[2] = s3;

        local_file_ptrs[3] = fs_ptr;
        local_file_ptrs += 4;
        __uint8_t f4 = GET_NTH_INPUT(eight_inputs, 6);
        fs_ptr += f4;
        local_file_sizes[3] = f4;
        local_file_sizes += 4;

        local_space_ptrs[3] = fs_ptr;
        local_space_ptrs += 4;
        __uint8_t s4 = GET_NTH_INPUT(eight_inputs, 7);
        fs_ptr += s4;
        local_space_sizes[3] = s4;
        local_space_sizes += 4;
    }
    
    __uint16_t *moar_input = (__uint16_t *)input;
    for (__uint8_t i = INPUT_SIZE & 7; i >= 2; i -= 2)
    {
        __uint16_t two_inputs = (*(moar_input++)) - (((__uint16_t)'0' << 8) | '0');
        *(local_file_ptrs++) = fs_ptr;
        __uint8_t f = two_inputs;
        fs_ptr += f;
        *(local_file_sizes++) = f;
        *(local_space_ptrs++) = fs_ptr;
        __uint8_t s = GET_NTH_INPUT(two_inputs, 1);
        fs_ptr += s;
        *(local_space_sizes++) = s;
    }
    
    if (INPUT_SIZE & 1) {
        *local_file_ptrs = fs_ptr;
        *local_file_sizes = (*((char *) moar_input) - '0');
    }
    
    memset(search_caches, 0, sizeof(search_caches));
}

void print_arrays() {
    printf("File sizes:\n");
    for (size_t i = 0; i < NUMBER_OF_FILES; i++)
    {
        printf("%d ", file_sizes[i]);
    }
    
    printf("\n\nSpace sizes:\n");
    for (size_t i = 0; i < NUMBER_OF_SPACES; i++)
    {
        printf("%d ", space_sizes[i]);
    }
    
    printf("\n\nFile pointers:\n");
    for (size_t i = 0; i < NUMBER_OF_FILES; i++)
    {
        printf("%d ", file_ptrs[i]);
    }
    
    printf("\n\nSize pointers:\n");
    for (size_t i = 0; i < NUMBER_OF_SPACES; i++)
    {
        printf("%d ", space_ptrs[i]);
    }
    printf("\n");
}
