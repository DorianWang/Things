#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

struct my_msg_st {
   long int my_msg_type;
   char some_text[BUFSIZ];
};

// Server
int main()
{
   int running = 1;
   int msgid;
   struct my_msg_st some_data;
   long int msg_to_receive = 0;
   int client_counter = 0; // Number of clients that have started communications with the server.

   msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

   if (msgid == -1){
      fprintf(stderr, "msgget failed with error: %d\n", errno);
      exit(EXIT_FAILURE);
   }

   while(running){
      if (msgrcv(msgid, (void*) &some_data, BUFSIZ, msg_to_receive, 0) == -1){
         fprintf(stderr, "msgrcv failed with error: %d\n", errno);
         exit(EXIT_FAILURE);
      }

      if (some_data.my_msg_type == 2){
         ++client_counter;
      }
      else if (some_data.my_msg_type == 3){
         --client_counter;
         if (client_counter == 0){
            running = 0;
         }
      }
      else
      {
         printf("You wrote: %s", some_data.some_text);
      }
   }

   if (msgctl(msgid, IPC_RMID, 0) == -1){
      fprintf(stderr, "msgctl(IPC_RMID) failed\n");
      exit(EXIT_FAILURE);
   }

   exit(EXIT_SUCCESS);
}





