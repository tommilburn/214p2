all: main

main: main.c
		gcc -Wall -o main main.c

clean:
		rm -rf *o main
