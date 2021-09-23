#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SERVER_FIFO_NAME "/tmp/serv_fifo"
#define CLIENT_FIFO_NAME "/tmp/cli_%d_fifo" // Variable name, has space for a number

#define BUFFER_SIZE 20

struct data_to_pass_st {
   pid_t client_pid;
   char some_data[BUFFER_SIZE - 1];
};

#endif // CLIENT_H_INCLUDED
