#ifndef SEM_FUNC_H_INCLUDED
#define SEM_FUNC_H_INCLUDED

#include <stdio.h>

#include <errno.h>
#include <sys/sem.h>

union semun {
   int val;
   struct semid_ds *buf;
   unsigned short *array;
};

extern int sem_id; // Follow the same structure as sem1.c, not sure if good for normal code.

int set_semvalue(int sem_id, int val);
void del_semvalue(int sem_id);
int semaphore_p(int sem_id);
int semaphore_v(int sem_id);

#endif // SEM_FUNC_H_INCLUDED

