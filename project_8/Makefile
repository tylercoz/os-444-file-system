CC := gcc
CFLAGS = -Werror -Wextra -Wall -DCTEST_ENABLE
OBJS = testfs.o image.o block.o free.o inode.o pack.o mkfs.o dir.o
.PHONY: all clean pristine

testfs: mylib.a
	$(CC) $(CFLAGS) -o $@ $^

mylib.a: $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^

test: testfs
	./testfs

clean:
	rm -f *.o

pristine: clean
	rm -f testfs *.a *.h.pch

debug: mylib.a
	$(CC) -g $(CFLAGS) -o $@ $^
