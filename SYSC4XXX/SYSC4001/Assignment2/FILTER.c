#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "sem_func.h"

// 7 characters and the null terminator
#define STRING_LENGTH 8
#define INPUT_BUFFER_SIZE 512

#define P1_SIG 0
#define P3_SIG 1
#define SV_ONE 2
#define SV_TWO 3
#define START_SIG 4
#define END_SIG 5

// Ascii check for digit. Might work with unicode, but I'm not bothering with that.
#define IS_DIGIT(x) (x >= '0' && x <= '9')
#define SWAP_CHAR(x, y) {char temp = x; x = y; y = temp;}





int main()
{
   int sem_id;
   int shmid;
   int i;
   int process_number;
   // Six semaphores, 2 for end signaling, 2 for locks, and 2 for starting and stopping.
   sem_id = semget((key_t)1234, 6, 0666 | IPC_CREAT);

   // This should create a shared memory space which can hold the string and the debug flag.
   shmid = shmget(IPC_PRIVATE, (sizeof(char) * (STRING_LENGTH + 1)), 0666 | IPC_CREAT);
   if (shmid == -1){
      fprintf(stderr, "shmget failed\n");
      exit(EXIT_FAILURE);
   }
   for (i = 0; i < 6; i++){
      // Sets all semaphores to 0, will be incremented later.
      if (!set_semvalue(sem_id, 0, i)){
         fprintf(stderr, "Semaphore value setting failed\n");
         exit(EXIT_FAILURE);
      }
   }

   // Create 3 children, with process_number of 0, 1 and 2.
   for (i = 0; i < 3; i++){
      pid_t pid_val = fork();
      switch(pid_val)
      {
         case -1:
            perror("fork failed");
            exit(EXIT_FAILURE);
         case 0: // I am a child
            process_number = i;
            i = 3; // just break out of the loop this way.
            break;
         default:
            process_number = -1;
            break;
      }
   }

   if (process_number == -1){ // This is the parent, managing process.
      int status;
      char* char_array = (char*) shmat(shmid, (void*) 0, 0);
      char prompt_buffer[INPUT_BUFFER_SIZE];

      printf("Would you like to use debug mode? (y/n): ");
      fgets(prompt_buffer, INPUT_BUFFER_SIZE, stdin);
      if (prompt_buffer[0] == 'y' || prompt_buffer[0] == 'Y'){
            printf("Debug mode ON.\n");
         char_array[STRING_LENGTH] = 1;
      }
      else if (prompt_buffer[0] == 'n' || prompt_buffer[0] == 'N'){
         printf("Debug mode OFF.\n");
         char_array[STRING_LENGTH] = 0;
      }
      else{
         printf("Continuing with debug mode OFF.\n");
         char_array[STRING_LENGTH] = 0;
      }

      while(1){
         char valid = 1;
         printf("Please enter a 7 character string to be filtered: ");
         fgets(prompt_buffer, INPUT_BUFFER_SIZE, stdin);
         for (i = 0; i < STRING_LENGTH - 1; i++){
            if (prompt_buffer[i] == 0){ // If the string is not 7 characters long, reprompt
               valid = 0;
            }
         }
         if (valid){
            //printf("%s\n", prompt_buffer);
            prompt_buffer[STRING_LENGTH - 1] = 0;
            memcpy((char_array), (prompt_buffer), STRING_LENGTH);
            break;
         }
      }

      // Set semaphores, and then tell processes to run.
      if (!semaphore_v(sem_id, SV_ONE)) exit(EXIT_FAILURE);
      if (!semaphore_v(sem_id, SV_TWO)) exit(EXIT_FAILURE);
      if (!semaphore_v(sem_id, P1_SIG)) exit(EXIT_FAILURE);
      if (!semaphore_v(sem_id, P3_SIG)) exit(EXIT_FAILURE);

      if (!semaphore_v(sem_id, START_SIG)) exit(EXIT_FAILURE);
      if (!semaphore_v(sem_id, START_SIG)) exit(EXIT_FAILURE);
      if (!semaphore_v(sem_id, START_SIG)) exit(EXIT_FAILURE);

      /*
      for (i = 0; i < 20; i++){
         int j;
         sleep(1);
         for (j = 0; j < 6; j++){
            printf("%d ", semaphore_check(sem_id, j));
         }
         printf("\n");
      }
      */

      // Wait on the end semaphore, which should be set by P2 if it detects no swaps and it is done.
      if (!semaphore_p(sem_id, END_SIG)){
         exit(EXIT_FAILURE);
      }

      for (i = 0; i < 3; i++){ // Wait for all children to exit.
         wait(&status);
         if (!WIFEXITED(status)){
            printf("Something bad happened to a child?\n");
         }
      }
      printf("The sorted list is: %s\n", char_array);
      // Cleanup is at the very bottom of the function, after this else block.
   }
   else { // Bubblesort, go! Numbers bubble up.
      char* char_array = (char*) shmat(shmid, (void*) 0, 0);
      char* local_array = char_array + process_number * 2; // Shifted pointer to simplify code.
      char swapped = 1; // Starts set so the array is always checked once.
      char running_state = 1; // 0 is stopped, 1 is running, 2 is check before done.

      if (!semaphore_p(sem_id, START_SIG)) exit(EXIT_FAILURE); // Wait for starting flag.
      char debug_set = char_array[STRING_LENGTH];

      if (process_number == 1){ // This is the second process, the one in the middle.
         while(running_state){
            if (!swapped){ // There were no swaps this time, check to see if the other two are done.
               if (debug_set) printf("P2 no swaps\n");
               if (running_state == 2) break; // Job's done.
               // These two checks will be true if both P1 and P3 think they are done.
               int left_done = semaphore_is_zero(sem_id, P1_SIG);
               int right_done = semaphore_is_zero(sem_id, P3_SIG);
               if (!left_done || !right_done){
                  exit(EXIT_FAILURE); // Check failed, panic!
               }
               else if (left_done == 1 && right_done == 1)
               {
                  // If nothing is swapped in the next check, quit.
                  running_state = 2;
               }
            }
            else
            {
               running_state = 1; // Reset back to regular running state.
            }
            swapped = 0;

            // Try to swap the centre value to the left
            if (!IS_DIGIT(local_array[1])){
               if (!semaphore_p(sem_id, SV_ONE)) exit(EXIT_FAILURE); // Get lock
               if (IS_DIGIT(local_array[0])){
                  SWAP_CHAR(local_array[0], local_array[1]);
                  if (!semaphore_v(sem_id, SV_ONE)) exit(EXIT_FAILURE); // Release lock
                  swapped = 1;
                  if (debug_set) printf("P2 swapping\n");
               }
               else
               {
                  if (!semaphore_v(sem_id, SV_ONE)) exit(EXIT_FAILURE); // Release lock
               }
            }

            // Try to swap the right value to the centre
            // This inverts the checks so that it doesn't lock unless needed.
            if (IS_DIGIT(local_array[1])){
               if (!semaphore_p(sem_id, SV_TWO)) exit(EXIT_FAILURE); // Get lock
               if (!IS_DIGIT(local_array[2])){
                  SWAP_CHAR(local_array[1], local_array[2]);
                  if (!semaphore_v(sem_id, SV_TWO)) exit(EXIT_FAILURE); // Release lock
                  swapped = 1;
                  if (debug_set) printf("P2 swapping\n");
               }
               else
               {
                  if (!semaphore_v(sem_id, SV_TWO)) exit(EXIT_FAILURE);
               }
            }
         }
         // Set end to 2 so both the blocked main process,
         // and the two other processes can proceed towards finishing.
         if (!set_semvalue(sem_id, 2, END_SIG)) exit(EXIT_FAILURE);
      }
      else
      {
         // If the requirements allowed it, I would include some additional optimisations.
         // Mostly just having an index for sorted values so each iteration would be smaller.
         // I'm not sure how to do the swap for all three values, since the array would underflow.

         if (process_number == 0){
            // Check the higher value first to pull down letters.
            // This might save 1 whole operation per execution, really making my program go zoom!
            while(running_state){

               if (!semaphore_p(sem_id, SV_ONE)) exit(EXIT_FAILURE); // Get lock
               if (!IS_DIGIT(local_array[2])){
                  if (IS_DIGIT(local_array[1])){
                     SWAP_CHAR(local_array[1], local_array[2]);
                     swapped = 1;
                     if (debug_set) printf("P1 swapping\n");
                  }
               }
               if (!semaphore_v(sem_id, SV_ONE)) exit(EXIT_FAILURE); // Release lock

               if (!IS_DIGIT(local_array[1])){
                  if (IS_DIGIT(local_array[0])){
                     SWAP_CHAR(local_array[0], local_array[1]);
                     swapped = 1;
                     if (debug_set) printf("P1 swapping\n");
                  }
               }

               if (swapped == 0){
                  if (running_state == 1){
                     running_state = 2;
                  }
                  else if(running_state == 2){
                     if (!semaphore_p(sem_id, P1_SIG)) exit(EXIT_FAILURE); //Take down "active" flag
                     running_state = 3;
                  }
                  else // running_state == 3
                  {
                     int val;

                     val = semaphore_is_zero(sem_id, END_SIG); // If END_SIG, kill process.
                     if (val){
                        if (val == 2){
                           break; // Not zero, stop!
                        }
                        // Value is zero, just keep going.
                        if (debug_set) printf("P1 no swaps\n");
                     }
                     else
                     {
                        exit(EXIT_FAILURE); // semop failed!
                     }
                  }
               }
               else // If there was a swap this time, reset running state.
               {
                  if (running_state == 3){
                     if (!semaphore_v(sem_id, P1_SIG)) exit(EXIT_FAILURE); // Put back "active" flag
                  }
                  running_state = 1;
               }
               swapped = 0;
            }
         }
         else
         {
            // Check the centre value first to pull up numbers.
            while(running_state){

               if (!IS_DIGIT(local_array[1])){
                  if (!semaphore_p(sem_id, SV_TWO)) exit(EXIT_FAILURE); // Get lock
                  if (IS_DIGIT(local_array[0])){
                     SWAP_CHAR(local_array[0], local_array[1]);
                     swapped = 1;
                     if (debug_set) printf("P3 swapping\n");
                  }
                  if (!semaphore_v(sem_id, SV_TWO)) exit(EXIT_FAILURE); // Release lock
               }

               if (!IS_DIGIT(local_array[2])){
                  if (IS_DIGIT(local_array[1])){
                     SWAP_CHAR(local_array[1], local_array[2]);
                     swapped = 1;
                     if (debug_set) printf("P3 swapping\n");
                  }
               }

               if (swapped == 0){
                  if (running_state == 1){
                     running_state = 2;
                  }
                  else if(running_state == 2){
                     if (!semaphore_p(sem_id, P3_SIG)) exit(EXIT_FAILURE); //Take down "active" flag
                     running_state = 3;
                  }
                  else // running_state == 3
                  {
                     int val;
                     val = semaphore_is_zero(sem_id, END_SIG); // If END_SIG, kill process.
                     if (val){
                        if (val == 2){
                           break; // Not zero, stop!
                        }
                        // Value is zero, just keep going.
                        if (debug_set) printf("P3 no swaps\n");
                     }
                     else
                     {
                        exit(EXIT_FAILURE); // semop failed!
                     }
                  }
               }
               else // If there was a swap this time, reset running state.
               {
                  if (running_state == 3){
                     if (!semaphore_v(sem_id, P3_SIG)) exit(EXIT_FAILURE); // Put back "active" flag
                  }
                  running_state = 1;
               }
               swapped = 0;
            }
         }
      }
      exit(EXIT_SUCCESS); // Worker process is complete!
   }

   // Delete semaphore set and shared memory.
   del_semvalue(sem_id);
   shmctl(shmid, IPC_RMID, NULL); // Mark to be removed, happens after exit() call.

   exit(EXIT_SUCCESS);
}
