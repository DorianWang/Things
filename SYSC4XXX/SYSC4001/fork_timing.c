
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>

#define MICRO_SEC_IN_SEC 1000000

int main()
{
   pid_t pid;
   char* message;
   int n;

   printf("fork program starting\n");
   pid = fork();
   switch(pid)
   {
   case -1:
      perror("fork failed");
      exit(1);
   case 0:
      fork()
      message = "This is the child";
      break;
   default:
      message = "This is the parent";
      n = 3;
      break; // This one might not be strictly necessary.
   }

   for(; n > 0; n--){
      puts(message);
      sleep(1);
   }
   exit(0);
}
