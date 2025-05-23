#include <unistd.h>
#include "mkfs.h"
#include "image.h"
#include "block.h"
#include "inode.h"

void mkfs() {
  image_open("disk.img", 1);
  unsigned char block[BLOCK_SIZE];
  for (int i = 0; i <= 6; i++) {
    bwrite(i, block);
  }
  // reserve first 7 blocks in free block map as used
  char first_seven = 127;
  lseek(image_fd, FREE_BLOCK_MAP * BLOCK_SIZE, SEEK_SET);
  write(image_fd, &first_seven, sizeof first_seven);

  struct inode *in = ialloc();
  int alloc_block = alloc();
  (void)alloc_block;

  in->flags = 2;
  in->size  = 2 *


  image_close();
}
