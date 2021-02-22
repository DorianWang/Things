#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "row_calc.h"
#include "test_case_constants.h"

#define MICRO_SEC_IN_SEC 1000000



int main()
{
   struct timeval start, end; // Timing variables, to time each test case.
   pid_t children[MATRIX_SIZE_CONST]; // Hold children, to keep track of them.
   int fd[2]; // File descriptors for pipe to transfer largest number in row.

   // This should create a memory space which can hold all 3 matrices, and a semaphore.
   int shmid;
   shmid = shmget(IPC_PRIVATE, (sizeof(m_4x4_t) * 3) + sizeof(sem_t),
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

   // Pointer arithmetic, for when you hate the future.
   // To future me, this gives the pointer to the location immediately after the 3rd matrix.
   // Then it sets the semaphore to 0 so that all children are blocked.
   sem_t* matrices_ready_semaphore = (sem_t*)(matrix_array + 4);
   sem_init(matrices_ready_semaphore, 1, 0);


   MMultStruct* tests_array;
   int total_test_cases = assemble_test_cases(&tests_array);
   int test_num;
   for(test_num = 0; test_num < total_test_cases; test_num++){

      gettimeofday(&start, NULL);
      pipe(fd);

      int i; int j;// I'm pretty sure this isn't needed for modern C, but old versions need it.
      for (i = 0; i < MATRIX_SIZE_CONST; i++){
         pid_t pid_val = fork();
         switch(pid_val)
         {
         case -1:
            perror("fork failed");
            exit(EXIT_FAILURE);
         case 0: // I am the child
            close(fd[0]); // Children close the reader end of the pipe.

            sem_wait(matrices_ready_semaphore);
            printf("Working with row %d!\n", i);
            MMultStruct temp = {(matrix_array), (matrix_array + 1), (matrix_array + 2)};
            int return_val = calc_row_mmult(i, temp);
            write(fd[1], &return_val, sizeof(int)); // Should be thread safe/atomic?
            close(fd[1]); // Close the pipe when you're done, good etiquette.
            exit(EXIT_SUCCESS);
         default: // Parent
            children[i] = pid_val;
         }
      }

      close(fd[1]); // The parent closes the writer end of their pipe.

      memcpy((matrix_array), *(tests_array[test_num].first), sizeof(m_4x4_t));
      memcpy((matrix_array + 1), *(tests_array[test_num].second), sizeof(m_4x4_t));

      //Allow children to run. If semaphores aren't used it is possible for children to race ahead.
      //An alternative solution would be to put the matrices in before the fork() calls.
      for (i = 0; i < MATRIX_SIZE_CONST; i++){
         sem_post(matrices_ready_semaphore);
      }

      //Wait for all children to return, and then try to read the number from the pipe.
      //If the number is bigger than the current highest number, then use it instead.
      int max_val = INT_MIN;
      for (i = 0; i < MATRIX_SIZE_CONST; i++){
         waitpid(children[i], NULL, 0);
         int row_max;
         read(fd[0], &row_max, sizeof(int));
         //printf("Value found was %d\n", row_max);
         if (row_max > max_val){
            max_val = row_max;
         }
      }
      close(fd[0]); // Close the pipe, don't want to make the kernal angry.
      gettimeofday(&end, NULL);

      printf("The time taken (for the forks and computation): %dus\n",
             (end.tv_sec - start.tv_sec) * MICRO_SEC_IN_SEC + (end.tv_usec - start.tv_usec));
      printf("The two input matrices were:\n");
      print_m_4x4_t(matrix_array + 0);
      printf("------------------------------------\n");
      print_m_4x4_t(matrix_array + 1);
      printf("The output matrix is:\n");
      print_m_4x4_t(matrix_array + 2);
      printf("The maximum value in the output is %d!\n\n", max_val);

   }

   printf("Program is complete!\n");
   return EXIT_SUCCESS;
}


