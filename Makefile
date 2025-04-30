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
.PHONY: all clean pristine

testfs: mylib.a
	gcc -o $@ $^

mylib.a: testfs.o image.o block.o
	ar rcs $@ $^

testfs.o: testfs.c
	$(CC) $(CLFAGS) -c $^

image.o: image.c image.h
	$(CC) $(CLFAGS) -c $^

block.o: block.c block.h
	$(CC) $(CLFAGS) -c $^

test: testfs
	./testfs

clean:
	rm -f testfs.o image.o block.o

pristine: clean
	rm testfs *.a
