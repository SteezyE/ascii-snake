CC=g++
CLIBS=-lncursesw
CFLAGS=-Wall -Wextra -Ofast

snake: snake.c snake.h
	$(CC) $(CFLAGS) -o snake snake.c $(CLIBS)

.PHONY: clean 
clean: 	
	rm snake
