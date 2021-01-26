
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>

#include <string.h>

#ifdef _WIN32
   #define wait(X) (X - X)
   #define fork() (0)
#else
   // __linux__ basically
   #include <sys/wait.h>
#endif

#define MICRO_SEC_IN_SEC 1000000
#define MAX_DEPTH 5

int main()
{
   pid_t pid;
   char* message = "Depth:%1d - PID:%5d - Child PID:%5d - Time Elapsed [fork()]: %4dus\n";
   int n;
   int depth = 0;
   struct timeval start, end;

   printf("fork program starting\r\n");
   printf("A process with depth n+1 is the child of process n.\r\n");
   gettimeofday(&start, NULL);


   while(depth <= MAX_DEPTH && depth >= 0){ // This should stop by MAX_DEPTH iterations, this is just an extra precaution

      pid = fork();

      switch(pid)
      {
      case -1:
         perror("fork failed");
         exit(1);
      case 0:
         depth = depth + 1;
         if (depth < MAX_DEPTH){
            continue;
         }
         //No break, so then the finishing statements run.
      default:
         gettimeofday(&end, NULL);
         n = 1;
         goto finish; // This might not be strictly necessary.
      }
   }

   finish:
      for(; n > 0; n--){
         sleep(1);
      }
      printf(message, depth, getpid(), pid, ((end.tv_sec - start.tv_sec) * MICRO_SEC_IN_SEC + (end.tv_usec - start.tv_usec)));
   if (pid == 0){
      exit(EXIT_SUCCESS);
   }

   int returnStatus;
   wait(&returnStatus); // This should make sure that all child processes end first so the console stays tidy.
   exit(returnStatus);
}
