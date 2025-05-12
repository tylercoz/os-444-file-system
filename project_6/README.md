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
* `ctest.h` - Testing suite by Beej
* `test_file.txt` - File for testing in `ctest.h`

## Data

 `image_fd` - global variable representing simulated image disk
* `BLOCK_SIZE` - global constant representing size of a block in our file system

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
* `free.c` 
  * `set_free- set a specific bit to the value in set (0 or 1)
  * `find_low_clear_bit` - bitwise operation helper function to find lowest clear bit 
  * `find_free`  - find a 0 bit and return its index (i.e. the block number that corresponds to this bit).


## Notes

* get tires rotated.
* believe in yourself. 
