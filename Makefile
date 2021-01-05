run: compile
	./autocomplete "oo"

compile:
	gcc autocomplete.c -o autocomplete -Wall -g
