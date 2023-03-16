CC=cc
CFLAGS=-I -std=c99 -pedantic -Wall
OBJ = main.o

server: $(OBJ)
	$(CC) -o server $(OBJ)
	chmod 755 server
	./server

clean:
	rm -f *.o server