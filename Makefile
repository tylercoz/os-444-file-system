CC := gcc
CFLAGS = -Werror -Wextra -Wall -DCTEST_ENABLE
OBJS = testfs.o image.o block.o
.PHONY: all clean pristine

testfs: mylib.a
	$(CC) $(CFLAGS) -o $@ $^

mylib.a: $(OBJS)
	ar rcs $@ $^

testfs.o: testfs.c
	$(CC) $(CFLAGS) -c $^

image.o: image.c image.h
	$(CC) $(CFLAGS) -c $^

block.o: block.c block.h
	$(CC) $(CFLAGS) -c $^

test: testfs
	./testfs

clean:
	rm -f $(OBJS)

pristine: clean
	rm -f testfs *.a *.h.pch
