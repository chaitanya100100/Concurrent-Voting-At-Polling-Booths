SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
CC = gcc
FLAGS = -lpthread
EXEC = election

all:$(EXEC)

$(EXEC) : $(OBJ) $(FLAGS)
	$(CC) $^ -o $@

%.o : %.c voting.h $(FLAGS)
	$(CC) -c $<

#.PHONY: clean
clean:
	rm -f $(OBJ) $(EXEC)
