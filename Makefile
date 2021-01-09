run: compile
	./autocomplete "LEASEFINANCE"

compile:
	gcc autocomplete.c -o autocomplete -Wall -g
