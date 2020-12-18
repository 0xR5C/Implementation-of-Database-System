#paths
LIB= ../lib 
INCLUDE= ../include 


#compiler
CC= gcc

#flags
CFLAGS= -Wall -Werror -g -I$(INCLUDE)
FL= -no-pie


OBJS= main.o heap.o $(LIB)/BF_64.a

EXEC= main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(FL) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC)