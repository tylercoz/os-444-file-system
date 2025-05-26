#include <stdio.h>
#include <stdlib.h>
#include  <string.h>

#include "dir.h"
#include "inode.h"
#include "block.h"
#include "pack.h"

struct directory *directory_open(int inode_num) {
  struct inode *in = iget(inode_num);
  if (in == NULL) {
    return NULL;
  }
  struct directory *dir = (struct directory *)malloc(sizeof(struct directory));
  dir->inode = in;
  dir->offset = 0;

  return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent){
  //check offsett against size of directory to check if its the end
  if (dir->offset >= dir->inode->size){
    return -1;
  }

  //get index to block_ptr to read
  int data_block_index = dir->offset / BLOCK_SIZE; 
  //read the block 
  int data_block_num = dir->inode->block_ptr[data_block_index];
  unsigned char block[BLOCK_SIZE];
  bread(data_block_num, block);

  //compute offset 
  int offset_in_block = dir->offset % BLOCK_SIZE;

  //extract directory entry 
  ent->inode_num = read_u16(block + offset_in_block);

  //extract the file name
  strcpy(ent->name, (char*)(block + offset_in_block + 2));

  //update position 
  dir->offset += sizeof(struct directory_entry);
  return 0; 
}



// double pointer so we can change d to NULL and test that d has been
// freed successfully
void directory_close(struct directory **d) {
  iput((**d).inode);
  free(*d);
  *d = NULL;
}
