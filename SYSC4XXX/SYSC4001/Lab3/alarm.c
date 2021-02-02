#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired = 0;

void ding(int sig)
{
   alarm_fired = 1;
}

int main()
{
   pid_t pid;
   struct sigaction act;
   act.sa_handler = ding;
   sigemptyset(&act.sa_mask);
   act.sa_flags = 0;
   sigaction(SIGALRM, &act, 0); // Hopefully this will connect the signal up

   printf("alarm application starting\n");

   pid = fork();

   switch(pid){
   case -1:
      perror("fork failed");
      exit(1);
   case 0:
      //Child
      sleep(5);
      kill(getppid(), SIGALRM);
      exit(0);
   }

   //Ideally this would put the thread to sleep and be unscheduled
   //until the alarm signal wakes it, instead of wasting CPU cycles.
   pause();
   if (alarm_fired)
      printf("Ding!\n");

   printf("done\n");
   exit(0);
}









