CC=gcc
FLAGS=-Wall -O

all: fifo1 fifo2 server client

fifo1: fifo1.c
	$(CC) $(FLAGS) -o $@ $^

fifo2: fifo2.c
	$(CC) $(FLAGS) -o $@ $^

server: server.c client.h
	$(CC) $(FLAGS) -o $@ $^

client: client.c client.h
	$(CC) $(FLAGS) -o $@ $^

