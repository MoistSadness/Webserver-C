CC=cc
CFLAGS=-I -std=c99 -pedantic -Wall
OBJ = main.o

# The default command does everything, it clears the terminal, compiles the app, and runs it. Just type make!
server: $(OBJ)
	clear
	$(CC) -o server $(OBJ)
	chmod 755 server
	./server

# Run the application. This assumes it is already compiled.
run:
	./server

# Clear out all those ramdon *.o files littering the source directory!
clean:
	rm -f *.o server