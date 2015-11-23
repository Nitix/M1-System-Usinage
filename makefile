CC=gcc
CFLAGS=-Wall
all:
	$(CC) entrepot.c -o entrepot $(CFLAGS)
	$(CC) generateur.c -o generateur $(CFLAGS)
	$(CC) machine1.c -o machine1 $(CFLAGS)
	$(CC) machine2.c -o machine2 $(CFLAGS)