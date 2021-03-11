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


int set_semvalue(int sem_id, int val, int sem_index);
void del_semvalue(int sem_id);
int semaphore_p(int sem_id, int sem_index);
int semaphore_v(int sem_id, int sem_index);

int semaphore_check(int sem_id, int sem_index);
int semaphore_is_zero(int sem_id, int sem_index);

#endif // SEM_FUNC_H_INCLUDED

