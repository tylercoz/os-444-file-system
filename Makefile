CC := gcc
CFLAGS = -Werror -Wextra -Wall
OBJS = testfs.o image.o block.o
.PHONY: all clean pristine

testfs: mylib.a
	gcc -o $@ $^

mylib.a: $(OBJS)
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
	rm -f $(OBJS)

pristine: clean
	rm -f testfs *.a
