#include "free.h"
#include "block.h"

void set_free(unsigned char *block, int num, int set) {
  int byte_num = num / 8;
  int bit_num = num % 8;

  if (set) {
    block[byte_num] |= (1 << bit_num);
  } else {
    block[byte_num] &= ~(1 << bit_num);
  }
}

int find_low_clear_bit(unsigned char x) {
  for (int i = 0; i < 8; i++) {
    if (!(x & (1 << i))) {
      return i;
    }
  }
  return -1;
}

int find_free(unsigned char *block) {
  for (int i = 0; i < BLOCK_SIZE; i++) {
    unsigned char byte = block[i];
    return find_low_clear_bit(byte);
  }
  return -1;
}
