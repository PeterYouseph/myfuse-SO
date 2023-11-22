CC = gcc
CFLAGS = -D_FILE_OFFSET_BITS=64 

all: myfs

myfs: myfs.c gitcurl.c
	$(CC) $(CFLAGS) myfs.c gitcurl.c -o myfs `pkg-config fuse --cflags --libs` -lcurl

clean:
	rm -f myfs