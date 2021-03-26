#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#include "text_storage.h"

char running;



/*
void text_storage_tester()
{
   int res;
   setvbuf(stdout, (char*)NULL, _IONBF, 0);
   TextStore* ts = create_store();
   DEBUG_PRINT(ts);
   store_append(ts, "Test1");
   DEBUG_PRINT(ts);

   for(int i = 0; i < 8; i++){
      store_append(ts, "Test1!");
   }

   for(int i = 0; i < 7; i++){
      res = store_delete(ts, "Test1");
      if (res == 0){
         printf("Not found?\n");
      }
      if (i % 2){
         DEBUG_PRINT(ts);
      }
   }
   DEBUG_PRINT(ts);
   delete_store(ts);


}

*/


void sig_handler(int sig)
{
   if (sig == SIGINT){
      running = 0;
      return;
   }
}


void setup(int* rec_msgid, int* snd_msgid, TextStore** ts)
{
   *rec_msgid = msgget((key_t)RECIEVE_QUEUE_ID, 0666 | IPC_CREAT);
   if (*rec_msgid == -1){
      fprintf(stderr, "msgget failed with error: %d\n", errno);
      exit(EXIT_FAILURE);
   }
   *snd_msgid = msgget((key_t)SEND_QUEUE_ID, 0666 | IPC_CREAT);
   if (snd_msgid == -1){
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

   *ts = create_store();

   running = 1;
}

void cleanup(int rec_msgid, int snd_msgid, TextStore* ts)
{
   char success = 1;
   delete_store(ts);
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
   int rec_msgid; int snd_msgid; TextStore* ts;
   setup(&rec_msgid, &snd_msgid, &ts);

   struct my_msg_st some_data;
   long int msg_to_receive = 0;
   int res;

   while(running){
      if (msgrcv(rec_msgid, (void*) &some_data, BUFSIZ, msg_to_receive, 0) == -1){
         if (errno == EINTR){
            break; // Interrupt caught, clean up.
         }
         fprintf(stderr, "msgrcv failed with error: %d\n", errno);
         break;
      }

      switch (some_data.my_msg_type)
      {
      case APPEND_E:
         res = store_append(ts, some_data.some_text);
         some_data.my_msg_type = res + 1;
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         break;
      case DELETE_E:
         res = store_delete(ts, some_data.some_text);
         some_data.my_msg_type = res + 1;
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         break;
      case REMOVE_E:
         res = store_remove(ts, some_data.some_text);
         some_data.my_msg_type = res + 1;
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         break;
      case SEARCH_E:
         const char* temp = store_search(ts, some_data.some_text);
         if (temp != NULL){
            some_data.my_msg_type = SUCCESS_E;
            memcpy(some_data.some_text, temp, MAX_TEXT_LENGTH);
         }
         else{
            some_data.my_msg_type = FAILURE_E;
         }
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         break;
      default:
         fprintf(stderr, "Message type not valid.\n");
         break;
      }
   }

   cleanup();
   exit(EXIT_SUCCESS);
}








