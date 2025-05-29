# Very Very Simple File System

## Building

* `make` - build executable `testfs`
* `make test` - `make` and run executable
* `make clean` - remove object files
* `make pristine` - `make clean` and remove `.a`, `.pch`, and executable

## Files

* `image.c` - open and close image disk
* `block.c` - Read and write to blocks in image disk
* `testfs.c` - Testing functionality. Run with `make test`
* `free.c` - set and find free blocks
* `inode.c` - inode functionality
* `dir.c` - directory operations
* `ls.c` - directory listing function
* `ctest.h` - Testing suite by Beej
* `pack.c` - Byte packing unpacking functions by Beej
* `test_file.txt` - File for testing in `ctest.h`

## Data

* `image_fd` - global variable representing simulated image disk
* `BLOCK_SIZE` - global constant representing size of a block in our file system
* `incore` - array storing virtual inode structs for easy use

## Functions

* `image.c`
  * `image_open` - open filename and return file descriptor
  * `image_close` - close global var image_fd
* `block.c`
  * `bread` - read from block num, put inside buffer passed in and return pointer to buffer passed in
  * `bwrite` - write buffer passed in to global var image_fd
  * `alloc` - allocate a previously-free data block from the block map.
* `inode.c`
  * `ialloc` - allocate a previously-free inode in the inode map
  * `incore_find_free` - finds next inode with no references and returns it
  * `incore_find` - return pointer to inode with passed in `inode_num`, or `-1` if none found.
  * `incore_free_all` - sets all incore inodes `ref_count` to `0`.
  * `write_inode` - write data from virtual inode struct onto image_fd
  * `read_inode` - read inode from image_fd onto virtual inode struct
  * `iget` - Return a pointer to an in-core inode for the given `inode_num`, or `NULL` on failure.
  * `iput` - decrement the `ref_count` on the inode. If it falls to `0`, write the inode to disk.
* `free.c`
  * `set_free` - set a specific bit to the value in set (0 or 1)
  * `find_low_clear_bit` - bitwise operation helper function to find lowest clear bit
  * `find_free` - find a 0 bit and return its index (i.e. the block number that corresponds to this bit).
* `dir.c`
  * `directory_open` - opens a directory and returns a directory struct
  * `directory_get` - reads the next directory entry
  * `directory_close` - closes an open directory
* `ls.c`
  * `ls` - lists directory contents (no testing required)


## Notes

* write thank you letter to Jim and Kathy
* be kind