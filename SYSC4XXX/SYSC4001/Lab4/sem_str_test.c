#include <unistd.h>
#include <stdlib.h>

#include "sem_func.h"

#define KEY_ONE ((key_t)1234)
#define KEY_TWO ((key_t)1235)

int main(int argc, char* argv)
{
   int ret_val;
   int i;
   int main_sem, other_sem;


   errno = 0;
   // Could try to learn about semaphore sets, but my brain is dead now.
   main_sem = semget(KEY_ONE, 2, 0666 | IPC_CREAT | IPC_EXCL);
   if (main_sem == -1){
      if (errno == EEXIST){ // This is the second/slower instance of the program.
         main_sem = semget(KEY_TWO, 2, 0666 | IPC_CREAT | IPC_EXCL);
         if (main_sem == -1){
            exit(EXIT_FAILURE);
         }
         other_sem = semget(KEY_ONE, 0, 0666); // Get existing semaphore of first instance.
         if (other_sem == -1){
            del_semvalue(main_sem);
            fprintf(stderr, "Failed to obtain second semaphore set identifier\n");
            exit(EXIT_FAILURE);
         }

         set_semvalue(main_sem, 0);
         if (!semaphore_v(other_sem)){ // Signal first instance.
            del_semvalue(main_sem);
            exit(EXIT_FAILURE);
         };
      }
      else
      {
         exit(EXIT_FAILURE);
      }
   }
   else // This would be the first/faster instance of the program.
   {
      if (!set_semvalue(main_sem, 0)) exit(EXIT_FAILURE);
      if (!semaphore_p(main_sem)) exit(EXIT_FAILURE); // Wait for the other program to finish initialization.

      other_sem = semget(KEY_TWO, 0, 0666);
      if (other_sem == -1){
         del_semvalue(main_sem);
         fprintf(stderr, "Failed to obtain second semaphore set identifier\n");
         exit(EXIT_FAILURE);
      }
      if (!semaphore_v(main_sem)) exit(EXIT_FAILURE);
   }

   // At this point the first semaphore should be 1, and the second semaphore should be 0.

   for (i = 0; i < 8; i++){
      if (!semaphore_p(main_sem)) exit(EXIT_FAILURE);
      printf("Hello I am process %d writing line %d.\n", (int)getpid(), i);
      errno = 0;
      if (!semaphore_v(other_sem)) exit(EXIT_FAILURE);
   }

   // The first instance will reach here first, and will need to signal one last time to sync up closing.
   if (!semaphore_v(other_sem)) exit(EXIT_FAILURE);
   if (!semaphore_p(main_sem)) exit(EXIT_FAILURE);


   del_semvalue(main_sem); // Delete the created semaphore.


   printf("Hello I am process %d, closing!\n", (int)getpid());
   exit(EXIT_SUCCESS);
}





