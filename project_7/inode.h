#ifndef INODE_H
#define INODE_H

#define INODE_PTR_COUNT 16
struct inode {
    unsigned int size;
    unsigned short owner_id;
    unsigned char permissions;
    unsigned char flags;
    unsigned char link_count;
    unsigned short block_ptr[INODE_PTR_COUNT];

    unsigned int ref_count;  // in-core only
    unsigned int inode_num;  // in-core only
};

int ialloc();
struct inode *incore_find_free();
struct inode *incore_find(unsigned int inode_num);
void incore_free_all();

#endif
