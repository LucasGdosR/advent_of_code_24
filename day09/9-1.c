#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void main(int argc, char *argv[]) {
    // ___________________________________________________
    // Read input
    // ___________________________________________________
    int fd = open("9-input", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    char *input = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);

    // ___________________________________________________
    // Calculate file system capacity
    // ___________________________________________________
    __uint32_t fs_capacity = 0;
    for (__uint16_t i = 0; i < sb.st_size; i++) {
        char int_repr = input[i] - '0';
        input[i] = int_repr;
        fs_capacity += int_repr;
    }    

    // ___________________________________________________
    // Init contents:
    //  file id, or -1 if free;
    //  cache sum of file sizes;
    // ___________________________________________________
    __int32_t fs_contents[fs_capacity];
    // memset takes a 4 byte int, and this is reading 1 byte chars
    // however, either way it is represented as all 1s in binary
    memset(fs_contents, -1, sizeof(fs_contents));

    __uint32_t fs_size = 0;
    {
        __uint32_t fs_idx = 0;
        __uint16_t i = 0;
        for (; i < sb.st_size - 1; i += 2) {
            char file_size = input[i];
            char free_space = input[i + 1];
            fs_size += file_size;

            __uint16_t f_id = i >> 1;
            for (__uint8_t i = 0; i < file_size; i++) {
                fs_contents[fs_idx++] = f_id;
            }
            fs_idx += free_space;
        }
        if (sb.st_size & 1) {
            char file_size = input[sb.st_size - 1];
            fs_size += file_size;
            __uint16_t f_id = sb.st_size >> 1;
            for (__uint8_t i = 0; i < file_size; i++) {
                fs_contents[fs_idx++] = f_id;
            }
        }
    }

    // ___________________________________________________
    // Scan files from the end, spaces from the start.
    // ___________________________________________________
    __uint32_t left = 0, right = fs_capacity;
    while (left < fs_size) {
        __int32_t fid = fs_contents[--right];
        if (fid > 0) {
            while (fs_contents[left++] >= 0);
            fs_contents[left - 1] = fid;
        }
    }
    
    // ___________________________________________________
    // Checksum
    // ___________________________________________________
    __uint64_t checksum = 0;
    for (__uint32_t i = 0; i < fs_size; i++) {
        checksum += i * fs_contents[i];
    }
    
    printf("Checksum: %ld\n", checksum);
}
