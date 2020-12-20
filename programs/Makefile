#paths
LIB= ../lib
INCLUDE= ../include

#compiler
CC= gcc

#flags
CFLAGS= -Wall -Werror -g -I$(INCLUDE)
FL= -no-pie


OBJS= main.o heap.o $(LIB)/BF_64.a
#LIBB= $(LIB)/BF_64.a

EXEC= main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIBB) $(FL) -o $(EXEC)

$(LIB)/BF_64.a:


clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)