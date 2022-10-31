#include <zlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define BLOCK_SIZE (1024*1024) // 1MB
uint8_t buf[BLOCK_SIZE];

int main(int argc, char** argv){
    if (argc != 2 && argc != 3) {
        printf("Usage: %s input_file [start_offset]\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1], O_RDONLY | O_CLOEXEC);
    if(fd < 0) {
        perror("Cannot open input file");
        return -1;
    }

    int current_block = 0;
    if (argc > 2) {
        char *endptr = NULL;
        size_t offset = strtoull(argv[2], &endptr, 10);
        if (*endptr != 0) {
            fprintf(stderr, "Cannot parse '%s' as integer\n", argv[2]);
            return -1;
        }
        printf("Offset %llu\n", offset);
        if (lseek(fd, offset*BLOCK_SIZE, SEEK_SET) < 0){
            perror("Seek failed");
            return -1;
        }
        current_block = offset;
    }

    int eof = 0;
    while (!eof) {
        size_t bytes_read = 0;
        while (bytes_read != BLOCK_SIZE) {
            ssize_t ret = read(fd, buf+bytes_read, BLOCK_SIZE-bytes_read);
            if(ret == 0) {
                eof = 1;
                break;
            }
            if(ret < 0) {
                if (errno == EINTR)
                    continue;
                printf("Block read error\n");
                perror("Block read error");
                return -1;
            }
            bytes_read += ret;
        }
        if (eof && bytes_read == 0)
            break;
        unsigned long crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, buf, bytes_read);
        printf("Block %d: %08X\n", current_block, crc);
        fflush(stdout);
        current_block++;
    }

    close(fd);

    return 0;
}
