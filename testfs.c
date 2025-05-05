#include <stdio.h>
#include "ctest.h"
#include "image.h"

// Testing
#ifdef CTEST_ENABLE

void test_image_open(){
  char *test_file = "test_file.txt";

  //open file no truncate
  image_fd = image_open(test_file, 0);
  CTEST_ASSERT(image_fd >= 0, "file open should succeed");
  image_close();

  //open file yes truncate
  image_fd = image_open(test_file, 1);
  CTEST_ASSERT(image_fd >= 0, "file open should succeed");
  image_close();

}

void test_image_close(){
  CTEST_ASSERT(image_close() == 0, "file should close");
}

int main() {
  CTEST_VERBOSE(1);

  test_image_open();
  test_image_close();


  CTEST_RESULTS();

  CTEST_EXIT();
}

// Production
#else


int main() {
  printf("hello, mother!\n");
}

#endif
