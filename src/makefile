# Para compilar: make
# Para limpar: make clean
# Para executar: ./myfs -f [diretorio de montagem]
# Para verificação se o diretório foi criado com o myfs : df -h ou df -T 
# Para desmontar: fusermount -u [diretorio de montagem]

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