#ifndef BLOCK_H
#define BLOCK_H

#define BLOCK_COUNT (BLOCK_SIZE * 8)
#define FREE_BLOCK_MAP 2

unsigned char *bread(int block_num, unsigned char *block);
void bwrite(int block_num, unsigned char *block);
int alloc();

extern const int BLOCK_SIZE;

#endif
