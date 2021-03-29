CC=gcc
FLAGS=-Wall -O

all: SERVER CLIENT

SERVER: server.c text_storage.c text_storage.h message_queue_consts.h
	$(CC) $(FLAGS) -o SERVER $^

CLIENT: client.c message_queue_consts.h
	$(CC) $(FLAGS) -o CLIENT $^
