#ifndef _REENTRANT
   #define _REENTRANT
#endif // _REENTRANT

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void* thread_function(void* arg);
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];


int main(){
   int res;
   pthread_t a_thread;
   void* thread_result;

   // Semaphore initialization before threading important?
   // First value is pointer to semaphore to be initialized.
   // Second value of 0, means that the semaphore is not shared between processes.
   // Third value of 0, starting value of the semaphore.
   res = sem_init(&bin_sem, 0, 0);
   if (res != 0){
      perror("Semaphore initialization failed");
      exit(EXIT_FAILURE);
   }
   res = pthread_create(&a_thread, NULL, thread_function, NULL);
   if (res != 0){
      perror("Thread creation failed");
      exit(EXIT_FAILURE);
   }

   printf("Input some text. Enter 'end' to finish.\n");
   while(strncmp("end", work_area, 3) != 0){
      fgets(work_area, WORK_SIZE, stdin);
      sem_post(&bin_sem);  // Signal()
   }
   printf("\nWaiting for thread to finish...\n");
   res = pthread_join(a_thread, &thread_result);
   if (res != 0){
      perror("Thread join failed");
      exit(EXIT_FAILURE);
   }
   printf("Thread joined\n");
   sem_destroy(&bin_sem);
   exit(EXIT_SUCCESS);
}

void* thread_function(void *arg){
   sem_wait(&bin_sem);  // Wait()
   while(strncmp("end", work_area, 3) != 0){
      printf("You input %ld characters\n", strlen(work_area) - 1);
      sem_wait(&bin_sem);  // Wait()
   }
   pthread_exit(NULL);
}

