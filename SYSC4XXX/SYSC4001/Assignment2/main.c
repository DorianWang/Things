#include <stdio.h>
#include <stdlib.h>

#include "sem_func.h"

int main()
{
   int sem_id;
   sem_id = semget((key_t)1234, 2, 0666 | IPC_CREAT);




    printf("Hello world!\n");
    return 0;
}
