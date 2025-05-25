#include <stdio.h>
#include <stdlib.h>

#include "dir.h"
#include "inode.h"

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

// double pointer so we can change d to NULL and test that d has been
// freed successfully
void directory_close(struct directory **d) {
  iput((**d).inode);
  free(*d);
  *d = NULL;
}
