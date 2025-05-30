#ifndef DIR_H
#define DIR_H

#define MAX_NAME_SIZE 16
#define ENTRY_SIZE 32
#define FILE_FLAG 1
#define DIR_FLAG 2

struct directory {
    struct inode *inode;
    unsigned int offset;
};

struct directory_entry {
    unsigned int inode_num;
    char name[MAX_NAME_SIZE];
};

struct directory *directory_open(int inode_num);
void directory_close(struct directory **d);
int directory_get(struct directory *dir, struct directory_entry *ent);
int directory_make(char *path);
#endif
