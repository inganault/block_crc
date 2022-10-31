#include <zlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BLOCK_SIZE (1024*1024)
uint8_t buf[BLOCK_SIZE];

int main(int argc, char** argv){
    if (argc != 2 && argc != 3){
        fprintf(stderr, "Usage: input_file [skip_block]\n");
        return -1;
    }

    int fd = open(argv[1], O_RDONLY | O_CLOEXEC);
    if(fd<0) {
        fprintf(stderr,"Error\n");
        return -1;
    }

    int block = 0;
    if (argc > 2){
        size_t offset = atoll(argv[2]);
        if(lseek(fd, offset*BLOCK_SIZE, SEEK_SET)<0){
            fprintf(stderr,"Seek Error\n");
            return -1;
        }
        block = offset;
    }

    int eof = 0;
    while(!eof){
        size_t readd = 0;
        while(readd != BLOCK_SIZE){
            int t = read(fd, buf+readd, BLOCK_SIZE-readd);
            if(t==0) {
                eof=1;
                break;
            }
            if(t<0) {
                fprintf(stderr,"Error\n");
                printf("Error\n");
                return -1;
            }
            readd += t;
        }
        unsigned long crc = crc32(0L, Z_NULL, 0);
        crc = crc32(crc, buf, readd);
        printf("Block %d: %08X\n", block, crc);
        fflush(stdout);
        block++;
    }

    close(fd);

    return 0;
}