#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ctest.h"
#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"

// Testing
#ifdef CTEST_ENABLE

void test_image_open() {
  char *test_file = "test_file.txt";
  int fd;

  //open file no truncate
  fd = image_open(test_file, 0);
  CTEST_ASSERT(fd >= 0, "file open should succeed with no truncate");
  image_close();

  //open file yes truncate
  fd = image_open(test_file, 1);
  CTEST_ASSERT(fd >= 0, "file open should succeed with truncate");
  image_close();

  fd = image_open(test_file, 1);
  CTEST_ASSERT(fd == image_fd, "global image_fd exists and is accurate");
  image_close();

}

void test_image_close() {
  CTEST_ASSERT(image_close() == 0, "file closes successfully");
}

void test_block_bwrite_and_bread() {
  char *test_file = "test_file.txt";

  // Writes data from a buffer to disk image
  unsigned char test_string[4096] = "A sentence, perhaps";
  unsigned char result[4096] = {0};
  image_open(test_file, 1);
  bwrite(0, test_string);
  bread(0, result);
  CTEST_ASSERT(strcmp((char *)result, (char *)test_string) == 0, "bwrite and bread write and read from the buffer");
  image_close();

  // bread() returns pointer to same buffer
  image_open(test_file, 0);
  unsigned char *buffer_returned = bread(0, result);
  CTEST_ASSERT(result == buffer_returned, "bread returns pointer to same buffer");
  image_close();
}

void test_block_alloc(){
  image_open("test_fs", 1);

  unsigned char zero_block[4096] = {0};
  bwrite(2, zero_block);


  CTEST_ASSERT(image_fd != -1, "Open test image");

  int block1 = alloc(); 
  CTEST_ASSERT(block1 != -1, "first alloc should succeed");

  int block2 = alloc(); 
  CTEST_ASSERT(block2 != -1 && block2 != block1, "second alloc() returns different block");

  image_close(); 
}

void test_inode_ialloc(){
  image_open("test_fs", 1);

  unsigned char zero_block[4096] = {0};
  bwrite(1, zero_block);

  CTEST_ASSERT(image_fd != -1, "Open test image");
  
  int inode1 = ialloc(); 
  CTEST_ASSERT(inode1 != -1, "first alloc should succeed");

  int inode2 = ialloc(); 
  CTEST_ASSERT(inode2 != -1 && inode2 != inode1, "second alloc() returns different inode");

  image_close(); 
}


void test_free_c() {
  char *test_file = "test_file.txt";

  // set a bit
  {
    image_open(test_file, 1);
    unsigned char block [4096] = {0};
    set_free(block, 9, 1);
    CTEST_ASSERT(block[1] == 2, "set_free() can set a bit");
    image_close();
  }

  // free a bit
  {
    image_open(test_file, 1);
    unsigned char block [4096] = {0};
    memset(block, 0xFF, sizeof(block));
    set_free(block, 9, 0);
    CTEST_ASSERT(block[1] == 253, "set_free() can free a bit");
    image_close();
  }

  // find next free bit
  {
    int SET_BIT_NUM = 7;
    image_open(test_file, 1);
    unsigned char block [4096] = {0};
    memset(block, 0xFF, sizeof(block));
    set_free(block, SET_BIT_NUM, 0);
    int result = find_free(block);
    CTEST_ASSERT(result == SET_BIT_NUM, "find_free() finds next free bit");

    image_close();
  }
}

int main() {
  CTEST_VERBOSE(1);

  test_image_open();
  test_image_close();

  test_block_bwrite_and_bread();
  test_free_c();
  
  test_inode_ialloc();
  test_block_alloc();

  CTEST_RESULTS();

  CTEST_EXIT();
}

// Production
#else


int main() {
  printf("hello, mother!\n");
}

#endif
