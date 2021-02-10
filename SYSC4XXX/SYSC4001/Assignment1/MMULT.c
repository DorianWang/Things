#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "row_calc.h"
#include "test_case_constants.h"


#define SIG_CONTINUE 12


void sig_handler(int sig)
{
   if (sig == SIG_CONTINUE){
      return;
   }
}


int main()
{

   struct sigaction signaller;
   signaller.sa_handler = sig_handler;
   sigemptyset(&signaller.sa_mask);
   signaller.sa_flags = 0;
    To prevent children from racing ahead of parent, could be used to recycle threads if required.
   sigaction(SIG_CONTINUE, &signaller, 0);

   struct timeval start, end; // Timing variables, to time each test case.
   pid_t children[4]; // Hold children, to poke them awake
   int fd[2]; // Used to transfer largest number data through pipe.

   // This should create a memory space which can hold all 3 matrices.
   int shmid;
   shmid = shmget(IPC_PRIVATE, (sizeof(m_4x4_t) * 3),
                  0666 | IPC_CREAT);
   if (shmid == -1){
      fprintf(stderr, "shmget failed\n");
      exit(EXIT_FAILURE);
   }

   m_4x4_t* matrix_array;
   matrix_array = (m_4x4_t*) shmat(shmid, (void*) 0, 0);
   if (matrix_array == (m_4x4_t*) -1){
      fprintf(stderr, "shmat failed\n");
      exit(EXIT_FAILURE);
   }


   gettimeofday(&start, NULL);
   pipe(fd);
   int i; // I'm not actually sure this is needed for modern C, but some old versions need it.
   for (i = 0; i < 4; i++){
      pid_t pid_val = fork();
      switch(pid_val)
      {
      case -1:
         perror("fork failed");
         exit(1);
      case 0: // I am the child
         pause();
         setvbuf(stdout, NULL, _IONBF, 0); // Print to terminal immediately, instead of buffering.
         close(fd[0]); // Children close the reader end of the pipe.
         MMultStruct temp = {(matrix_array), (matrix_array + 1), (matrix_array + 2)};
         int return_val = calc_row_mmult(i, temp);
         write(fd[1], &return_val, sizeof(int)); // Should be thread safe/atomic?
         close(fd[1]); // Close the given pipe when you're done?
         break;
      default: // Parent
         children[i] = pid_val;
      }
   }
   close(fd[1]); // The parent closes the writer end of their pipe.

   //Tell children to run.
   for (i = 0; i < 4; i++){
      kill(children[i], SIG_CONTINUE);
   }

   //Wait for all children to return, and then try to read the number from the pipe.
   //If the number is bigger than the current highest number, then use it instead.
   int max_val = INT_MIN;
   for (i = 0; i < 4; i++){
      waitpid(children[i], NULL);
      int row_max;
      read(fd[0], &row_max, sizeof(int));
      if (row_max > max_val){
         max_val = row_max;
      }
   }
   close(fd[0]); // Close the pipe, just in case.
   printf("The maximum value in the output is %d!", max_val);

   printf("Program complete!\n");

   return 0;
}


