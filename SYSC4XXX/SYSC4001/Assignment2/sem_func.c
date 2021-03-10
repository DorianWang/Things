#include "sem_func.h"

int set_semvalue(int sem_id, int val, int sem_index)
{
   union semun sem_union;

   sem_union.val = val;
   if (semctl(sem_id, sem_index, SETVAL, sem_union) == -1) return (0);
   return(1);
}

void del_semvalue(int sem_id)
{
   union semun sem_union;
   //https://man7.org/linux/man-pages/man2/semctl.2.html
   if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
      fprintf(stderr, "Failed to delete semaphore\n");
}

int semaphore_p(int sem_id, int sem_index) // Wait()
{
   struct sembuf sem_b;

   sem_b.sem_num = sem_index;
   sem_b.sem_op = -1; // Something about P()
   sem_b.sem_flg = SEM_UNDO;
   if (semop(sem_id, &sem_b, 1) == -1){
      fprintf(stderr, "semaphore_p failed\n");
      return(0);
   }
   return(1);
}

int semaphore_v(int sem_id, int sem_index) // Signal()
{
   struct sembuf sem_b;

   sem_b.sem_num = sem_index;
   sem_b.sem_op = 1; // Something about V()
   sem_b.sem_flg = SEM_UNDO;
   errno = 0;
   if (semop(sem_id, &sem_b, 1) == -1){
      fprintf(stderr, "semaphore_v failed\n");
      return(0);
   }
   return(1);
}


