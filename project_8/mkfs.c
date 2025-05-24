#include <string.h>
#include <unistd.h>
#include "mkfs.h"
#include "image.h"
#include "block.h"
#include "inode.h"
#include "dir.h"
#include "pack.h"

#define ENTRY_SIZE 32

void mkfs() {
  image_open("disk.img", 1);
  unsigned char empty_block[BLOCK_SIZE];
  for (int i = 0; i <= 6; i++) {
    bwrite(i, empty_block);
  }
  // reserve first 7 blocks in free block map as used
  char first_seven = 127;
  lseek(image_fd, FREE_BLOCK_MAP * BLOCK_SIZE, SEEK_SET);
  write(image_fd, &first_seven, sizeof first_seven);

  // get inode and block
  struct inode *in = ialloc();
  int alloc_block_num = alloc();

  // set inode
  in->inode_num = 0;
  in->flags = 2;
  in->size = 2 * ENTRY_SIZE;
  in->block_ptr[0] = alloc_block_num;

  unsigned char block[BLOCK_SIZE];
  // this directory "."
  write_u16(block, in->inode_num);
  strcpy((char *)block, ".");

  // this directory ".."
  write_u16(block + ENTRY_SIZE, in->inode_num);
  strcpy((char *)block, "..");

  bwrite(alloc_block_num, block);
  iput(in);

  image_close();
}
