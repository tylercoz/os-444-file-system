#ifndef INODE_H
#define INODE_H

#define INODE_COUNT (BLOCK_SIZE * 8)
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3
#define INODE_BLOCKS 4
#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)
#define MAX_SYS_OPEN_FILES 64

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

struct inode *ialloc(void);
struct inode *incore_find_free();
struct inode *incore_find(unsigned int inode_num);
void incore_free_all();
void write_inode(struct inode *in);
void read_inode(struct inode *in, int inode_num);
struct inode *iget(int inode_num);
void iput(struct inode *in);

struct inode *namei(char *path);

#endif
