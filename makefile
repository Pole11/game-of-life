CC=gcc
CFLAGS=-g -Wall 
HEADERS=util.h
OBJ=game.o util.o 

%.o: %.c $(HEADERS) 
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS)