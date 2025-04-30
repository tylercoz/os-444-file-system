#ifndef IMAGE_H
#define IMAGE_H

unsigned char *bread(int block_num, unsigned char *block);
void bwrite(int block_num, unsigned char *block);

#endif