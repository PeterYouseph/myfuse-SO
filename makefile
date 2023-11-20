CC = gcc
CFLAGS = -D_FILE_OFFSET_BITS=64 

all: myfs

myfs: myfs.o
	$(CC) $(CFLAGS)  myfs.c -o myfs `pkg-config fuse --cflags --libs`
	echo 'To Mount: ./myfs -f [mount point]'

myfs.o: myfs.c
	$(CC) $(CFLAGS) -c myfs.c

clean:
	rm -f myfs myfs.o