CC = gcc
EXEC = test
FLAGS = -Wall -Werror -g
SRC = tests.c qio.c qstring.c
INCLUDE = qio.h qstring.h unittest.h

test: $(SRC) $(INCLUDE)
	$(CC) $(FLAGS) $(SRC) -o test

.PHONY: clean

clean:
	rm -f $(EXEC) *.o
