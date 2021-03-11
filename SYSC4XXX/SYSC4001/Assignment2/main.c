#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sem_func.h"

// 7 characters and the null terminator
#define STRING_LENGTH 8

#define P1_SIG 0
#define SV_ONE 1
#define P3_SIG 2
#define SV_2 3
#define END_SIG 4

// Ascii check for digit. Might work with unicode, but I'm not bothering with that.
#define IS_DIGIT(x) (x >= '0' && x <= '9')
#define SWAP_CHAR(x, y) {char temp = x; x = y; y = temp;}





int main()
{
   int sem_id;
   int shmid;
   int i;
   int process_number;
   // Four semaphores, 2 pairs and end signal. The first value in the pair is for the outer process,
   // while the second value in the pair is for the centre process to run.
   sem_id = semget((key_t)1234, 5, 0666 | IPC_CREAT);

   // This should create a shared memory space which can hold the string and the debug flag.
   shmid = shmget(IPC_PRIVATE, (sizeof(char) * (STRING_LENGTH + 1),
                  0666 | IPC_CREAT);
   if (shmid == -1){
      fprintf(stderr, "shmget failed\n");
      exit(EXIT_FAILURE);
   }
   for (i = 0; i < 5; i++){
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
            break;
         default:
            continue;
      }
      process_number = -1;
   }

   if (process_number == -1){ // This is the parent, managing process.
      int status;
      char* char_array = (char*) shmat(shmid, (void*) 0, 0);
      char prompt_buffer[8];

      printf("Would you like to use debug mode? (y/n): ");
      scanf("%c", prompt_buffer); // scanf() since it's easy.
      if (prompt_buffer[0] == 'y' || prompt_buffer[0] == 'Y'){
         char_array[STRING_LENGTH] = 1;
      }
      else if (prompt_buffer[0] == 'n' || prompt_buffer[0] == 'N'){
         char_array[STRING_LENGTH] = 0;
      }
      else{
         printf("\nContinuing without debug mode.");
         char_array[STRING_LENGTH] = 0;
      }

      while(1){
         printf("\nPlease enter a 7 character string to be filtered: ");
         scanf("%7s", prompt_buffer);
         for (i = 0; i < STRING_LENGTH, i++){
            if (prompt_buffer[i] == 0){ // If the string is not 7 characters long, reprompt
               continue;
            }
         }
         memcpy((char_array), (prompt_buffer), STRING_LENGTH);
         break;
      }

      // Tell the two outer processes to start running.
      if (!semaphore_v(sem_id, P1_SIG)) exit(EXIT_FAILURE);
      if (!semaphore_v(sem_id, P3_SIG)) exit(EXIT_FAILURE);

      // Wait on the end semaphore, which should be set by P2 if it detects no swaps and it is done.
      if (!semaphore_p(sem_id, END_SIG)){
         exit(EXIT_FAILURE);
      }

      for (i = 0; i < 3; i++){
         waitpid(&status);
         if (!WIFEXITED(status)){
            printf("Something bad happened to a child?\n");
         }
      }
   }
   else { // Bubblesort, go! Numbers bubble up.
      char* char_array = (char*) shmat(shmid, (void*) 0, 0);
      char* local_array = char_array + process_number * 2;

      if (process_number == 1){ // This is the second process, the one in the middle.
         char l_sorted = 0, r_sorted = 0;
         while(1){
            if (l_sorted && r_sorted){
               //Guess we're done here!
               break;
            }

            // Try to swap the centre value to the left if
            if (!IS_DIGIT(local_array[1]) && (!l_sorted)){

               if (!semaphore_p(sem_id, P2_TWO)) exit(EXIT_FAILURE);
               if (char_array[SV_TWO] >= '0' && char_array[SV_TWO] <= '9'){
                  r_sorted = 1; // No swaps were needed on the right side.
                  if (!semaphore_v(sem_id, P2_TWO)) exit(EXIT_FAILURE);
               }
               else
               { // Swap the centre value and the right value.
                  char temp = char_array[3];
                  char_array[3] = char_array[SV_TWO];
                  char_array[SV_TWO] = temp;
                  r_sorted = 0;
                  if (!semaphore_v(sem_id, P3_SIG)) exit(EXIT_FAILURE);
               }
            }

            // This is not a number, assume to be a letter. Try to swap to the left.
            if (rsorted || !(char_array[3] >= '0' && char_array[3] <= '9'){


               if (!semaphore_p(sem_id, P2_ONE)) exit(EXIT_FAILURE);
               if (!(char_array[SV_ONE] >= '0' && char_array[SV_ONE] <= '9')){
                  l_sorted = 1; // No swaps were needed on the left side.
                  if (!semaphore_v(sem_id, P2_ONE)) exit(EXIT_FAILURE);
               }
               else
               { // Swap the centre value and the left value.
                  char temp = char_array[3];
                  char_array[3] = char_array[SV_ONE];
                  char_array[SV_ONE] = temp;
                  l_sorted = 0;
                  if (!semaphore_v(sem_id, P1_SIG)) exit(EXIT_FAILURE);
               }
            }
            if (!semaphore_p(sem_id, P2_ONE)) exit(EXIT_FAILURE);
         }
         // Poke other processes so they die,
      }
      else
      {
         // If the requirements allowed it, I would include some additional optimisations.
         // Mostly just having an index for sorted values so each iteration would be smaller.
         // I'm not sure how to do the swap for all three values, since the array would underflow.
         int swapped = 0;

         if (process_number == 0){
            // Check the higher value first to pull down letters.
            // This might save 1 whole operation per execution, really making my program go zoom!

            while(1){
               if (!semaphore_p(sem_id, SV_ONE)) exit(EXIT_FAILURE);
               if (!IS_DIGIT(local_array[2])){
                  if (IS_DIGIT(local_array[1])){
                     SWAP_CHAR(local_array[1], local_array[2]);
                     // Tell P1 that there was a swap by signaling
                     if (!semaphore_v(sem_id, P1_SIG)) exit(EXIT_FAILURE);
                     swapped = 1;
                  }
               }
               if (!semaphore_v(sem_id, SV_ONE)) exit(EXIT_FAILURE);

               if (!IS_DIGIT(local_array[1])){
                  if (IS_DIGIT(local_array[0])){
                     SWAP_CHAR(local_array[0], local_array[1]);
                     swapped = 1;
                  }
               }

               if (swapped == 0){
                  if (semaphore_check(sem_id, P1_SIG) == 0 && semaphore_check(sem_id, P3_SIG) == 0){
                     // If this is true, both P1 and P3 are waiting on a semaphore signal.
                     // First set the end semaphore, then wake the two other processes.
                     if (!semaphore_v(sem_id, END_SIG)) exit(EXIT_FAILURE);
                     if (!semaphore_v(sem_id, P1_SIG)) exit(EXIT_FAILURE);
                     if (!semaphore_v(sem_id, P3_SIG)) exit(EXIT_FAILURE);
                  }
               }

            }
         }
         else
         {
            // Check the centre value first to pull up numbers.

         }

      }

   }


    printf("Hello world!\n");
    return 0;
}
