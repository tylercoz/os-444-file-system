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
  int block_offset_bytes = block_offset * INODE_SIZE;

  unsigned char block[BLOCK_SIZE];
  bread(block_num, block);

  write_u32(block + block_offset_bytes + 0, in->size);
  write_u16(block + block_offset_bytes + 4, in->owner_id);
  write_u8 (block + block_offset_bytes + 6, in->permissions);
  write_u8 (block + block_offset_bytes + 7, in->flags);
  write_u8 (block + block_offset_bytes + 8, in->link_count);


  for (int i = 0; i < INODE_PTR_COUNT; i++) {
    write_u16(block + block_offset_bytes + 9 + (2*i), in->block_ptr[i]);
  }

  bwrite(block_num, block);
}

void read_inode(struct inode *in, int inode_num) {
  int block_num = (inode_num / INODES_PER_BLOCK) + INODE_FIRST_BLOCK;
  int block_offset = inode_num % INODES_PER_BLOCK;
  int block_offset_bytes = block_offset * INODE_SIZE;

  unsigned char block[BLOCK_SIZE];
  bread(block_num, block);

  in->size        = read_u32(block + block_offset_bytes + 0);
  in->owner_id    = read_u16(block + block_offset_bytes + 4);
  in->permissions = read_u8 (block + block_offset_bytes + 6);
  in->flags       = read_u8 (block + block_offset_bytes + 7);
  in->link_count  = read_u8 (block + block_offset_bytes + 8);

  for (int i = 0; i < INODE_PTR_COUNT; i++) {
    in->block_ptr[i] = read_u16(block + block_offset_bytes + 9 + (2*i));
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
