#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>

#include "text_storage.h"
#include "message_queue_consts.h"

char running;

u_int8_t num_appends;
u_int64_t appends_time;
u_int8_t num_deletes;
u_int64_t deletes_time;
u_int8_t num_removes;
u_int64_t removes_time;
u_int8_t num_searchs;
u_int64_t searchs_time;


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

   *ts = create_store();

   running = 1;
}

void cleanup(int rec_msgid, int snd_msgid, TextStore* ts)
{
   char success = 1;
   delete_store(ts);
   printf("Average time for append: %lu us\n", appends_time/num_appends);
   printf("Average time for delete: %lu us\n", deletes_time/num_deletes);
   printf("Average time for remove: %lu us\n", removes_time/num_removes);
   printf("Average time for search: %lu us\n", searchs_time/num_searchs);
   if (msgctl(snd_msgid, IPC_RMID, 0) == -1){
      fprintf(stderr, "msgctl(IPC_RMID) failed\n");
      success = 0;
   } // Still try to remove the other message queue, hope for the best.
   if (msgctl(rec_msgid, IPC_RMID, 0) == -1){
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
   struct timeval start, end;
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

      gettimeofday(&start, NULL);

      switch (some_data.my_msg_type)
      {
      case APPEND_E:
         res = store_append(ts, some_data.some_text);
         some_data.my_msg_type = res + 1;
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         num_appends++;
         gettimeofday(&end, NULL);
         appends_time += (end.tv_sec * 1000000 + end.tv_usec)
                        - (start.tv_sec * 1000000 + start.tv_usec);
         break;
      case DELETE_E:
         res = store_delete(ts, some_data.some_text);
         some_data.my_msg_type = res + 1;
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         num_deletes++;
         gettimeofday(&end, NULL);
         deletes_time += (end.tv_sec * 1000000 + end.tv_usec)
                        - (start.tv_sec * 1000000 + start.tv_usec);
         break;
      case REMOVE_E:
         res = store_remove(ts, some_data.some_text);
         some_data.my_msg_type = res + 1;
         if (msgsnd(snd_msgid, (void*) &some_data, MAX_TEXT_LENGTH, 0) == -1){
            fprintf(stderr, "msgsnd failed\n");
            running = 0;
         }
         num_removes++;
         gettimeofday(&end, NULL);
         removes_time += (end.tv_sec * 1000000 + end.tv_usec)
                        - (start.tv_sec * 1000000 + start.tv_usec);
         break;
      case SEARCH_E:;
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
         num_searchs++;
         gettimeofday(&end, NULL);
         searchs_time += (end.tv_sec * 1000000 + end.tv_usec)
                        - (start.tv_sec * 1000000 + start.tv_usec);
         break;
      default:
         fprintf(stderr, "Message type not valid.\n");
         break;
      }
   }

   cleanup(rec_msgid, snd_msgid, ts);
   exit(EXIT_SUCCESS);
}








