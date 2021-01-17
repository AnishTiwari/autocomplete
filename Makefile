CFLAGS=-DDEBUG -g -Wall
WASM_ARG=oo

run: compile
	./autocomplete "oo"



compile:
	gcc autocomplete.c -o autocomplete $(CFLAGS)


wasi:
	wasicc autocomplete.c -o autocomplete.wasm

wasi-run: wasi
	wasmer --dir=. autocomplete.wasm $(WASM_ARG)

