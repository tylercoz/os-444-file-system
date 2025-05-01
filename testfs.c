#include <stdio.h>
#include "ctest.h"

// Testing
#ifdef CTEST_ENABLE

void test_testing(int x, int y) {
  (void) x;
  (void) y;
  CTEST_ASSERT(1, "testing testing 1");
  CTEST_ASSERT(1, "testing testing 2");
  CTEST_ASSERT(1, "testing testing 3");
  CTEST_ASSERT(1, "testing testing 4");
  CTEST_ASSERT(1, "testing testing 5");
}

int main() {
  CTEST_VERBOSE(1);

  test_testing(1, 4);

  CTEST_RESULTS();

  CTEST_EXIT();
}

// Production
#else

int main() {
  printf("hello, mother!\n");
}

#endif
