#include <fcntl.h>
#include <unistd.h>
#include "image.h"
int BLOCK_SIZE = 4096;

unsigned char *bread(int block_num, unsigned char *block){
    //lseek
    lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
    //read
    read(image_fd, block, BLOCK_SIZE);
    return block;
}

void bwrite(int block_num, unsigned char *block){
    //lseek
    lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
    //write
    write(image_fd, block, BLOCK_SIZE);
}
