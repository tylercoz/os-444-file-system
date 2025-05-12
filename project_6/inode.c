#include "inode.h"
#include "block.h" 
#include "free.h"


//const int BLOCK_SIZE = 4096; //bytes
#define INODE_COUNT (BLOCK_SIZE * 8)

int ialloc(void){
    //allocate a previously free inode in teh inode map 
    unsigned char block[BLOCK_SIZE]; 
    bread(1, block); 

    int inode_num = find_free(block);
    if(inode_num == -1 || inode_num >= INODE_COUNT ){
        return -1; 
    }

    set_free(block, inode_num, 1);
    bwrite(1, block);

    return inode_num; 

}