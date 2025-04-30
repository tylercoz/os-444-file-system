CC := gcc
CFLAGS = -Werror -Wextra -Wall
.PHONY: all clean

# all: testfs

# testfs: testfs.c
# 	$(CC) $(CFLAGS) -o $@ $^

# # Build tests and execute them, don't produce executable?
# test: testfs.c
# 	$(CC) $(CFLAGS) -o $@ $^

# clean:
# 	rm -f testfs test

all: testfs
.PHONY: all

testfs: testfs.o image.o block.o
	$(CC) -o $@ $^

testfs.o: testfs.c
	$(CC) $(CLFAGS) -c $^

image.o: image.c image.h
	$(CC) $(CLFAGS) -c $^

block.o: block.c block.h
	$(CC) $(CLFAGS) -c $^
