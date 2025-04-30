CC := gcc
CFLAGS = -Werror -Wextra -Wall
.PHONY: all clean

all: testfs

testfs: testfs.c
	$(CC) $(CFLAGS) -o $@ $^

# Build tests and execute them, don't produce executable?
test: testfs.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f testfs test
