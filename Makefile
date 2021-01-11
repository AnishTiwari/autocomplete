CFLAGS=-DDEBUG -g -Wall


run: compile
	./autocomplete "oo"



compile:
	gcc autocomplete.c -o autocomplete $(CFLAGS)
