#include <fcntl.h>
#include <unistd.h>
#include "image.h"
#include "block.h"
#include "free.h"

//extern variable that is shared with the other files
const int BLOCK_SIZE = 4096; //bytes
#define BLOCK_COUNT (BLOCK_SIZE * 8)

// These methods are writing and reading from the fake disk we created in image.c
// which is referred to by the file description `image_fd`

unsigned char *bread(int block_num, unsigned char *block) {
    //lseek
    lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
    //read
    read(image_fd, block, BLOCK_SIZE);
    return block;
}

void bwrite(int block_num, unsigned char *block) {
    //lseek
    lseek(image_fd, block_num * BLOCK_SIZE, SEEK_SET);
    //write
    write(image_fd, block, BLOCK_SIZE);
}

int alloc(void){
    unsigned char block[BLOCK_SIZE];
    bread(2, block);

    int block_num = find_free(block);
    if (block_num == -1 || block_num >= BLOCK_COUNT){
        return -1; 
    }

    set_free(block, block_num, 1);
    bwrite(2, block);

    return block_num; 

}