// This file tests all the other files
// New format: When adding a test, go to the function with the file name, add a comment on what function you are testing,
// and add brackets, so that the tests don't interact with each other.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ctest.h"
#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"
#include "mkfs.h"

// Testing
#ifdef CTEST_ENABLE

static char *test_file = "test_file.txt";

void test_image_c() {
  // open()
  {
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

  // close()
  {
    CTEST_ASSERT(image_close() == 0, "file closes successfully");
  }
}

void test_block_c() {

  // bwrite()
  {
    image_open(test_file, 1);
    unsigned char test_string[BLOCK_SIZE];
      const char *src = "A sentence, perhaps";
      memcpy(test_string, src, strlen(src));
    unsigned char result[BLOCK_SIZE];
    bwrite(0, test_string);
    bread(0, result);
    CTEST_ASSERT(strcmp((char *)result, (char *)test_string) == 0, "bwrite and bread write and read from the buffer");
    image_close();
  }

  // bread()
  {
    image_open(test_file, 0);
    unsigned char result[BLOCK_SIZE];
    unsigned char *buffer_returned = bread(0, result);
    CTEST_ASSERT(result == buffer_returned, "bread returns pointer to same buffer");
    image_close();
  }

  // alloc()
  {
    image_open(test_file, 1);

    unsigned char zero_block[BLOCK_SIZE];
    bwrite(2, zero_block);


    CTEST_ASSERT(image_fd != -1, "Open test image");

    int block1 = alloc();
    CTEST_ASSERT(block1 != -1, "first alloc should succeed");

    int block2 = alloc();
    CTEST_ASSERT(block2 != -1 && block2 != block1, "second alloc() returns different block");

    image_close();
  }
}

void test_inode_c() {


    image_open(test_file, 1);
    unsigned char zero_block[BLOCK_SIZE];
    memset(zero_block, 0, BLOCK_SIZE);
    bwrite(1, zero_block);

    //ialloc()
    {
      struct inode *inode1 = ialloc();

    CTEST_ASSERT(inode1 != NULL, "Open test image");

    CTEST_ASSERT(inode1->ref_count == 1, "ialloc should set ref_count to 1");

    CTEST_ASSERT(inode1->size==0, "ialloc should initialuze size to 0");

    CTEST_ASSERT(inode1->inode_num >= 0, "ialloc should set valid inode_num");

    struct inode *in2 = ialloc();
        CTEST_ASSERT(in2 != NULL && in2 != inode1, "ialloc() should return different inodes");


    // bitmap update gone right
    unsigned char block[BLOCK_SIZE];
        bread(1, block);
        int free_inode = find_free(block);
        CTEST_ASSERT(free_inode != (int)inode1->inode_num && free_inode != (int)in2->inode_num,"ialloc should mark inodes as used in bitmap");
    }


  // incore_find_free()
  // incore_find()
  // incore_free_all()
  {
    unsigned int test_ref_count = 69;
    unsigned int test_inode_num = 123;

    image_open(test_file, 1);
    incore_free_all();
    struct inode *free_inode = incore_find_free();
    free_inode->ref_count = test_ref_count;
    free_inode->inode_num = test_inode_num;
    struct inode *result_of_find = incore_find(test_inode_num);
    CTEST_ASSERT(free_inode == result_of_find, "incore_find() returns the correct inode");
    CTEST_ASSERT(result_of_find->ref_count == test_ref_count, "both functions return pointer that is useable");
    incore_free_all();
    struct inode *test_free_all = incore_find_free();
    CTEST_ASSERT(free_inode == test_free_all, "free_all() resets the incore");
    image_close();
  }


  // write_inode()
  // read_inode()
  {
    image_open(test_file, 1);
    struct inode *in = incore_find_free();
    struct inode *result = incore_find_free();
    in->size = 10;
    in->owner_id = 2;
    in->permissions = 1;
    in->flags = 7;
    in->link_count = 4;
    in->inode_num = 123;
    in->block_ptr[4] = 14;

    write_inode(in);
    read_inode(result, 123);

    CTEST_ASSERT(in->size == result->size, "inode size written to disk properly");
    CTEST_ASSERT(in->owner_id == result->owner_id, "inode owner id written to disk properly");
    CTEST_ASSERT(in->permissions == result->permissions, "inode permissions written to disk properly");
    CTEST_ASSERT(in->flags == result->flags, "inode flags written to disk properly");
    CTEST_ASSERT(in->link_count == result->link_count, "inode link count written to disk properly");
    CTEST_ASSERT(in->block_ptr[4] == result->block_ptr[4], "inode block ptrs written to disk properly");
    image_close();
  }

  //iget and iput
  {
    incore_free_all();
    unsigned test_inum = 50;

    //first iget loads from disk
    struct inode *inode1 = iget((unsigned int)(test_inum));
    CTEST_ASSERT(inode1 != NULL, "iget should succeed first time");
    CTEST_ASSERT(inode1->ref_count == 1, "iget should set ref_count to 1");
    CTEST_ASSERT(inode1->inode_num == test_inum, "iget should set correct inode_num");

    // Second iget should increment count
    struct inode *inode2 = iget((unsigned int)test_inum);
    CTEST_ASSERT(inode1 == inode2, "iget should return same pointer for same inode");
    CTEST_ASSERT(inode1->ref_count == 2, "iget should increment ref_count");

    // iput() tests
    iput(inode1);
    CTEST_ASSERT(inode1->ref_count == 1, "iput should decrement ref_count");

    iput(inode2);
    CTEST_ASSERT(inode1->ref_count == 0, "iput should decrement to 0");

    }

    image_close();
  }


void test_free_c() {

  // set_free()
  // set a bit
  {
    image_open(test_file, 1);
    unsigned char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);
    set_free(block, 9, 1);
    CTEST_ASSERT(block[1] == 2, "set_free() can set a bit");
    image_close();
  }

  // set_free()
  // free a bit
  {
    image_open(test_file, 1);
    unsigned char block [BLOCK_SIZE];
    memset(block, 0xFF, sizeof(block));
    set_free(block, 9, 0);
    CTEST_ASSERT(block[1] == 253, "set_free() can free a bit");
    image_close();
  }

  // find_free()
  {
    int SET_BIT_NUM = 7;
    image_open(test_file, 1);
    unsigned char block [BLOCK_SIZE];
    memset(block, 0xFF, sizeof(block));
    set_free(block, SET_BIT_NUM, 0);
    int result = find_free(block);
    CTEST_ASSERT(result == SET_BIT_NUM, "find_free() finds next free bit");

    image_close();
  }
}

void test_mkfs_c() {
  mkfs();
}

int main() {
  CTEST_VERBOSE(1);

  test_image_c();
  test_block_c();
  test_free_c();
  test_inode_c();
  test_mkfs_c();

  CTEST_RESULTS();

  CTEST_EXIT();
}

// Production
#else

int main() {
  printf("hello, mother!\n");
}

#endif
