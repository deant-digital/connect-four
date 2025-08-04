connect_four:
	gcc main.c game.c state.c ui.c -o connect_four

clean:
	rm -f connect_four *.o
