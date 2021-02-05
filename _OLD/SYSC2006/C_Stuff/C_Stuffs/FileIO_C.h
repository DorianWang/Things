#ifndef FILEIO_C_H_INCLUDED
#define FILEIO_C_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_MAX_READ 4096
//This module will use a buffer with a size of 4 kb.



struct library {
   unsigned int (*readFileText)(FILE* file, char***, unsigned int, unsigned int);
   unsigned int (*readFileData)(FILE* file, char**, unsigned int);
};

extern const struct library FileIO_C;



/* end interface.c */











#endif // FILEIO_C_H_INCLUDED
