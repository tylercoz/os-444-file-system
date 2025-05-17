#ifndef INODE_H
#define INODE_H

int ialloc();
struct inode *incore_find_free();
struct inode *incore_find(unsigned int inode_num);
void incore_free_all();

#endif
