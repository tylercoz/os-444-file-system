#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ctest.h"
#include "image.h"
#include "block.h"

// Testing
#ifdef CTEST_ENABLE

void test_image_open() {
  char *test_file = "test_file.txt";
  int fd;

  //open file no truncate
  fd = image_open(test_file, 0);
  CTEST_ASSERT(fd >= 0, "file open should succeed");
  image_close();

  //open file yes truncate
  fd = image_open(test_file, 1);
  CTEST_ASSERT(fd >= 0, "file open should succeed");
  image_close();

  fd = image_open(test_file, 1);
  CTEST_ASSERT(fd == image_fd, "global image_fd exists and is accurate");
  image_close();

}

void test_image_close() {
  CTEST_ASSERT(image_close() == 0, "file should close");
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

// }

int main() {
  CTEST_VERBOSE(1);

  test_image_open();
  test_image_close();

  test_block_bwrite_and_bread();

  CTEST_RESULTS();

  CTEST_EXIT();
}

// Production
#else


int main() {
  printf("hello, mother!\n");
}

#endif
