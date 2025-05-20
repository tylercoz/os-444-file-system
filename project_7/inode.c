#include <stdlib.h>
#include <stdio.h>

#include "inode.h"
#include "block.h"
#include "free.h"
#include "pack.h"

#define INODE_COUNT (BLOCK_SIZE * 8)
#define INODE_SIZE 64
#define INODE_FIRST_BLOCK 3
#define INODE_BLOCKS 4
#define INODES_PER_BLOCK (BLOCK_SIZE / INODE_SIZE)
#define MAX_SYS_OPEN_FILES 64

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};
static int incore_size = sizeof(incore) / sizeof(struct inode);

struct inode *ialloc(void){
    //allocate a previously free inode in teh inode map
    unsigned char block[BLOCK_SIZE];
    bread(1, block);

    int inode_num = find_free(block);
    if (inode_num == -1 || inode_num >= INODE_COUNT ) {
        return NULL;
    }

    //in-core version
    struct inode *in = iget(inode_num);

    set_free(block, inode_num, 1);
    bwrite(1, block);

    in->size = 0;
    in->owner_id = 0; 
    in->permissions = 0; 
    in->flags = 0; 
    in->link_count = 0; 
    for (int i = 0; i < INODE_PTR_COUNT; i++){
        in->block_ptr[i] = 0; 
    }

    in->inode_num = inode_num; 

    write_inode(in);

    return in;

}

struct inode *incore_find_free(void) {
  for (int i = 0; i < incore_size; i++) {
    if (incore[i].ref_count == 0) {
      return incore + i;
    }
  }
  return NULL;
}

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

void incore_free_all(void) {
  for (int i = 0; i < incore_size; i++) {
    incore[i].ref_count = 0;
  }
}

void write_inode(struct inode *in) {
  int inode_num = in->inode_num;

  int block_num = (inode_num / INODES_PER_BLOCK) + INODE_FIRST_BLOCK;
  int block_offset = inode_num % INODES_PER_BLOCK;

  unsigned char block[4096] = {0};
  bread(block_num, block);

  struct inode *din = (struct inode *)(block + block_offset);

  din->size = in->size;
  din->owner_id = in->owner_id;
  din->permissions = in->permissions;
  din->flags = in->flags;
  din->link_count = in->link_count;

  for (int i = 0; i < INODE_PTR_COUNT; i++) {
    din->block_ptr[i] = in->block_ptr[i];
  }

  bwrite(block_num, (unsigned char *)block);
}

void read_inode(struct inode *in, int inode_num) {
  int block_num = (inode_num / INODES_PER_BLOCK) + INODE_FIRST_BLOCK;
  int block_offset = inode_num % INODES_PER_BLOCK;

  unsigned char block[4096] = {0};
  bread(block_num, block);

  struct inode din = *(struct inode *)(block + block_offset);

  in->size = din.size;
  in->owner_id = din.owner_id;
  in->permissions = din.permissions;
  in->flags = din.flags;
  in->link_count = din.link_count;

  for (int i = 0; i < INODE_PTR_COUNT; i++) {
    in->block_ptr[i] = din.block_ptr[i];
  }
}

// TODO: TEST
struct inode *iget(int inode_num) {
  struct inode *in = incore_find(inode_num);
  if (in != NULL) {
    in->ref_count++;
    return in;
  }
  struct inode *free_inode = incore_find_free();
  if (free_inode == NULL) {
    // No free nodes left
    return NULL;
  }
  read_inode(free_inode, inode_num);
  free_inode->ref_count = 1;
  free_inode->inode_num = inode_num;
  return free_inode;
}


void iput(struct inode *in){
    if (in == NULL){
        return; 
    }

    if (in->ref_count == 0){
        return; 
    }

    in->ref_count--;

    if(in->ref_count == 0){
        write_inode(in);
    }
}