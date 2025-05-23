#include <fcntl.h>
#include <unistd.h>
#include "image.h"

int image_fd = -1;  // Initialize to an invalid fd


// returns file descriptor
int image_open(char *filename, int truncate){
  if (truncate) {
    image_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0600);
  }
  else {
    image_fd = open(filename, O_RDWR | O_CREAT, 0600);
  }
  return image_fd;
}

int image_close(void){
    if (image_fd != -1){
      close(image_fd);
    }
    return 0;
}
