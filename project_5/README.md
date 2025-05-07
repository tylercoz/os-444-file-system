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
* `ctest.h` - Testing suite by Beej
* `test_file.txt` - File for testing in `ctest.h`

## Data

* `image_fd` - global variable representing simulated image disk
* `BLOCK_SIZE` - global constant representing size of a block in our file system

## Functions

* `image.c`
  * `image_open` - open filename and return file descriptor
  * `image_close` - close global var image_fd
* `block.c`
  * `bread` - read from block num, put inside buffer passed in and return pointer to buffer passed in
  * `bwrite` - write buffer passed in to global var image_fd

## Notes

* Get milk from the store.
* life is awesome.
