#ifndef _REENTRANT
   #define _REENTRANT
#endif // _REENTRANT

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

char message[] = "Testing String!";

void* thread_function(void *arg){
   printf("%s\n", (char*)arg);
   pthread_exit(EXIT_SUCCESS);
}

/* The results of this test show that making threads is almost always faster, though sometimes
** the threaded solution is slower, which may be due to preemption by the OS.
** My device shows the process solution averages around 80us, while the threaded one about 65us.
*/
int main(){
   struct timeval start, end;
   pid_t temp_pid;
   gettimeofday(&start, NULL);
   temp_pid = fork();
   gettimeofday(&end, NULL);
   if (temp_pid == 0){
      printf("%s\n", message);
      exit(EXIT_SUCCESS);
   }
   if (temp_pid == -1){
      perror("Fork failed");
      exit(EXIT_FAILURE);
   }
   wait(NULL);

   printf("Process time: %ld us\n", ((end.tv_sec * 1000000 + end.tv_usec)
                    - (start.tv_sec * 1000000 + start.tv_usec)));

   pthread_t a_thread;

   gettimeofday(&start, NULL);
   pthread_create(&a_thread, NULL, thread_function, (void*)message);
   gettimeofday(&end, NULL);
   int* thread_result;
   pthread_join(a_thread, (void**)&thread_result);

   printf("Thread time:  %ld us\n", ((end.tv_sec * 1000000 + end.tv_usec)
                    - (start.tv_sec * 1000000 + start.tv_usec)));

   exit(EXIT_SUCCESS);
}
