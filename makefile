CC = gcc

tetris: puyopuyo.c
	$(CC) -o puyopuyo puyopuyo.c -lncurses

test:
	./puyopuyo
clean:
	rm -f puyopuyo

