CC=gcc
CFLAGS=-Wall
DIR=bin
all:
	mkdir -p bin
	$(CC) entrepot.c -o bin/entrepot $(CFLAGS)
	$(CC) generateur.c -o bin/generateur $(CFLAGS)
	$(CC) machine1.c -o bin/machine1 $(CFLAGS)
	$(CC) machine2.c -o bin/machine2 $(CFLAGS)