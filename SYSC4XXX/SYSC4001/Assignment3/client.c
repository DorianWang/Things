
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <sys/types.h>
#include <sys/msg.h>

#include "message_queue_consts.h"

char running;

void sig_handler(int sig)
{
   if (sig == SIGINT){
      running = 0;
      return;
   }
}


void setup(int* rec_msgid, int* snd_msgid)
{
   *rec_msgid = msgget((key_t)SEND_QUEUE_ID, 0666 | IPC_CREAT);
   if (*rec_msgid == -1){
      fprintf(stderr, "msgget failed with error: %d\n", errno);
      exit(EXIT_FAILURE);
   }
   *snd_msgid = msgget((key_t)RECIEVE_QUEUE_ID, 0666 | IPC_CREAT);
   if (*snd_msgid == -1){
      fprintf(stderr, "msgget failed with error: %d\n", errno);
      if (msgctl(*rec_msgid, IPC_RMID, 0) == -1){
         fprintf(stderr, "msgctl(IPC_RMID) failed\n");
         exit(EXIT_FAILURE);
      }
      exit(EXIT_FAILURE);
   }

   struct sigaction signaller;
   signaller.sa_handler = sig_handler;
   sigemptyset(&signaller.sa_mask);
   signaller.sa_flags = 0;
   sigaction(SIGINT, &signaller, 0); // Block SIGINT

   running = 1;
}

void cleanup(int rec_msgid, int snd_msgid)
{
   char success = 1;
   if (msgctl(snd_msgid, IPC_RMID, 0) == -1){
      fprintf(stderr, "msgctl(IPC_RMID) failed\n");
      success = 0;
   } // Still try to remove the other message queue, hope for the best.
   if (msgctl(msgid, IPC_RMID, 0) == -1){
      fprintf(stderr, "msgctl(IPC_RMID) failed\n");
      success = 0;
   }
   if (!success){
      exit(EXIT_FAILURE);
   }
}

int main()
{
   char command_buffer[TEXT_COMMAND_BUFFER]; char* input_slice_pointer;
   int rec_msgid; int snd_msgid;
   setup(&rec_msgid, &snd_msgid);

   while(running){
      printf("Enter some text: ");
      fgets(command_buffer, TEXT_COMMAND_BUFFER, stdin);
      input_slice_pointer = strtok(command_buffer, DELIMITERS);
      size_t command_len = strlen(input_slice_pointer);

      if(strcmp(input_slice_pointer, APPEND_STR_E) == 0){
         some_data.my_msg_type = APPEND_E;
      }
      else if (strcmp(input_slice_pointer, DELETE_STR_E) == 0){
         some_data.my_msg_type = DELETE_E;
      }
      else if (strcmp(input_slice_pointer, REMOVE_E) == 0){
         some_data.my_msg_type = REMOVE_STR_E;
      }
      else if (strcmp(input_slice_pointer, SEARCH_E) == 0){
         some_data.my_msg_type = SEARCH_STR_E;
      }
      else{
         printf("The entered command \'%s\' is not valid.\r\n", input_slice_pointer);
         some_data.my_msg_type = 0;
         continue;
      }

      strcpy(some_data.some_text, input_slice_pointer + command_len + 1);

      if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT, 0) == -1){
         fprintf(stderr, "msgsnd failed\n");
         running = 0;
      }
   }
   cleanup(rec_msgid, snd_msgid);
   exit(EXIT_SUCCESS);
}





