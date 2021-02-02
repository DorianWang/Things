#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include <signal.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/shm.h>

// Only check GCC, since that is the only one I will be working with.
// Won't work with pre-ANSI but ANSI C is the de-facto standard.
#if __GNUC__
   #if defined(__LP64__) || defined(_LP64)
      #define ENV_64_BIT
   #else
      #define ENV_32_BIT
   #endif
   // Not always true, but I don't care about other environments
#else
   #define ENV_32_BIT
#endif // __GNUC__

#define MAX_GENERATED_VALUE 9
#define THRESHOLD_VALUE 7

// These signal values should be unimplemented and safe for me to use.
#define SIG_HIGH_NUMBER 10
#define SIG_CONTINUE 12

static unsigned char found_high_number = 0;
static unsigned char stopped = 0;

/*
Thanking various stack overflow nerds:
https://stackoverflow.com/questions/1716296/why-does-printf-not-flush-after-the-call-unless-a-newline-is-in-the-format-strin
https://stackoverflow.com/questions/5950395/makefile-to-compile-multiple-c-programs
http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmget.html
https://stackoverflow.com/questions/8915230/invalid-application-of-sizeof-to-incomplete-type-with-a-struct
*/

// For sigint on parent, try to stop children as well.
void sig_handler(int sig)
{
   if (sig == SIGINT){
      stopped = 1;
      return;
   }

   if (sig == SIG_HIGH_NUMBER){
      found_high_number = 1;
      return;
   }

   if (sig == SIG_CONTINUE){
      return;
   }
}

void mark_high_number(int sig)
{
   found_high_number = 1;
}

void mark_continue(int sig)
{
   ; //Just for waking from pause()
}

int main()
{
   printf("Running random threshold program, searching for values higher than %d for random values between 0 and %d.\nThis program will stop if you press Ctrl-C (SIGINT)\n",
          THRESHOLD_VALUE, MAX_GENERATED_VALUE);

   struct sigaction signaller;
   signaller.sa_handler = sig_handler;
   sigemptyset(&signaller.sa_mask);
   signaller.sa_flags = 0;
   sigaction(SIGINT, &signaller, 0); // Shared SIGINT
   sigaction(SIG_HIGH_NUMBER, &signaller, 0); // Shared SIGINT
   sigaction(SIG_CONTINUE, &signaller, 0); // Shared SIGINT

   int shmid;
   shmid = shmget(IPC_PRIVATE, sizeof(uint32_t),
                  0666 | IPC_CREAT);

   if (shmid == -1){
      fprintf(stderr, "shmget failed\n");
      exit(EXIT_FAILURE);
   }

   pid_t cpid;
   cpid = fork();

   uint32_t *shared_int_address;
   shared_int_address = (uint32_t*) shmat(shmid, (void*) 0, 0);
   if (shared_int_address == (uint32_t*) -1){
      fprintf(stderr, "shmat failed\n");
      exit(EXIT_FAILURE);
   }

   switch (cpid){
   case -1:
      fprintf(stderr, "fork failed\n");
      exit(EXIT_FAILURE);
   case 0:; // Child
      setvbuf(stdout, NULL, _IONBF, 0); // Print to terminal immediately, instead of buffering.
      while(!stopped){
         *shared_int_address = rand() % (MAX_GENERATED_VALUE + 1); // A "random" value
         if (*shared_int_address >= THRESHOLD_VALUE){
            printf("\nChild: I found a high number of %d!\n", *shared_int_address);

            kill(getppid(), SIG_HIGH_NUMBER);
            pause();
            if (stopped){
               break; // Skip the incoming sleep
            }
         }
         else
         {
            printf("%d ", *shared_int_address);
         }
         sleep(1);
      }
      break;

   default:; // Probably Parent

      while(!stopped){

         pause();

         if (stopped){
            break;
         }

         if (found_high_number){
            printf("Parent: The child found a high number of %d!\n", *shared_int_address);
            found_high_number = 0;
            kill(cpid, SIG_CONTINUE);
         }
      }
      printf("Parent is killing child\n");
      kill(cpid, SIGINT);
      wait(NULL);
      break; // Not really needed
   }


   if (shmdt(shared_int_address) == -1){
      fprintf(stderr, "shmdt failed\n");
      exit(EXIT_FAILURE);
   }

   printf("Process is ending!\n");
   exit(EXIT_SUCCESS);
}

