#include <stdlib.h>

#include "inode.h"
#include "block.h"
#include "free.h"

#define INODE_COUNT (BLOCK_SIZE * 8)
int ialloc(void){
    //allocate a previously free inode in teh inode map
    unsigned char block[BLOCK_SIZE];
    bread(1, block);

    int inode_num = find_free(block);
    if (inode_num == -1 || inode_num >= INODE_COUNT ) {
        return -1;
    }

    set_free(block, inode_num, 1);
    bwrite(1, block);

    return inode_num;

}

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

#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3
#define INODE_BLOCKS 4
#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)

#define MAX_SYS_OPEN_FILES 64
static struct inode incore[MAX_SYS_OPEN_FILES] = {0};
static int incore_size = sizeof(incore) / sizeof(struct inode);

// TODO: TEST
struct inode *incore_find_free(void) {
  for (int i = 0; i < incore_size; i++) {
    if (incore[i].ref_count == 0) {
      return incore + i;
    }
  }
  return NULL;
}

// TODO: TEST
struct inode *incore_find(unsigned int inode_num) {
  for (int i = 0; i < incore_size; i++) {
    if (incore[i].ref_count != 0) {
      if (incore[i].inode_num == inode_num) {
        return incore + i;
      }
    }
  }
  return NULL;
}

// TODO: TEST
void incore_free_all(void) {
  for (int i = 0; i < incore_size; i++) {
    incore[i].ref_count = 0;
  }
}
