#ifndef FREE_H
#define FREE_H

// set a specific bit to the value in `set` (0 or 1)
// block: block 1 for free map or block 2 for inode map
// num:   bit number to set
//        0<=num<=BLOCK_SIZE * 8
// set:   0 or 1
void set_free(unsigned char *block, int num, int set);

//find a 0 bit and return its index (i.e. the block number that
//corresponds to this bit).
//block:   block 1 for free map or block 2 for inode map
int find_free(unsigned char *block);

#endif
