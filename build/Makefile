#paths
LIB= ../lib/
INCLUDE= ../include/
SRC= ../src/
BIN= ../bin/
EXAMPLES= ../examples/

#compiler
CC= gcc

#flags
CFLAGS= -Wall -Werror -g -I$(INCLUDE)
FL= -no-pie
ARGS= $(EXAMPLES)records1K.txt

OBJS= $(BIN)main.o $(BIN)heap.o
OBJS2= $(BIN)main_hash.o $(BIN)hash.o
LIBB= $(LIB)BF_64.a

EXEC= main

EXEC2= main_hash

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIBB) $(FL) -o $(EXEC)

$(EXEC2): $(OBJS2)
	$(CC) $(OBJS2) $(LIBB) $(FL) -o $(EXEC2)

$(LIB)BF_64.a:

$(BIN)heap.o: $(SRC)heap.c
	$(CC) -c $(CFLAGS) $(SRC)heap.c -o $(BIN)heap.o

$(BIN)main.o: $(SRC)main.c
	$(CC) -c $(CFLAGS) $(SRC)main.c -o $(BIN)main.o

$(BIN)hash.o: $(SRC)hash.c
	$(CC) -c $(CFLAGS) $(SRC)hash.c -o $(BIN)hash.o

$(BIN)main_hash.o: $(SRC)main_hash.c
	$(CC) -c $(CFLAGS) $(SRC)main_hash.c -o $(BIN)main_hash.o

clean:
	rm -f $(OBJS) $(OBJS2) $(EXEC) $(EXEC2)


heap: $(EXEC)
	./$(EXEC) $(ARGS)

hash: $(EXEC2)
	./$(EXEC2)

valgrind1: $(EXEC)
	valgrind ./$(EXEC)

valgrind2: $(EXEC2)
	valgrind ./$(EXEC2)
