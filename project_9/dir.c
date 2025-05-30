#include <stdio.h>
#include <stdlib.h>
#include  <string.h>

#include "dir.h"
#include "dirbasename.h"
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

int directory_make(char *path) {
  // check path starts with "/"
  if (path[0] != '/') {
    return -1;
  }

  // Find the directory path that will contain the new directory
  char *dirname[MAX_NAME_SIZE];
  get_dirname(path, (char *)dirname);

  // Find the new directory name from the path
  char *basename[MAX_NAME_SIZE];
  get_basename(path, (char *)basename);

  // Find the inode for the parent directory that will hold the new entry
  struct inode *parent = namei(path);

  // Create a new inode for the new directory
  struct inode *in = ialloc();

  // Create a new data block for the new directory entries
  int in_block_num = alloc();

  // Create a new block-sized array for the new directory data block
  unsigned char block[BLOCK_SIZE];

  // This directory "."
  write_u16(block, in->inode_num);
  strcpy((char *)block + 2, ".");

  // Parent directory ".."
  write_u16(block + ENTRY_SIZE, parent->inode_num);
  strcpy((char *)block + ENTRY_SIZE + 2, "..");

  // Initialize the new directory in-core inode
  in->flags = DIR_FLAG;
  in->size  = 2 * ENTRY_SIZE;
  in->block_ptr[0] = in_block_num;

  //Write the new directory data block to disk
  bwrite(in_block_num, block);

  // From the parent directory inode, find the block that will contain the new directory entry
  if (parent->size == (unsigned int)BLOCK_SIZE) {
    return -1;
  }
  int parent_block_num = parent->block_ptr[0];
  unsigned char parent_block[BLOCK_SIZE];

  // Read that block into memory
  bread(parent_block_num, parent_block);

  // Add the new directory entry to it
  write_u16(parent_block + parent->size, in->inode_num);
  strcpy((char *)parent_block + 2, (char *)basename);

  // Write that block to disk
  bwrite(parent_block_num, parent_block);

  // Update the parent directory's size field
  parent->size += ENTRY_SIZE;

  // Release the new directory's in-core inode
  iput(in);

  // Release the parent directory's in-core inode
  iput(parent);

  return 0;
}
