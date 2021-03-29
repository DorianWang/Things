#ifndef MESSAGE_QUEUE_CONSTS_H_INCLUDED
#define MESSAGE_QUEUE_CONSTS_H_INCLUDED
#include <stdio.h>

// From assignment recommendations:
// "Please ensure that each sentence transferred is no more than 35 characters in length."
#define MAX_TEXT_LENGTH 36
#define TEXT_COMMAND_BUFFER 64

// All whitespace characters in ascii, as well as some punctuation marks.
#define DELIMITERS " .,!?&/\\\t\r\n\v\f"

#define APPEND_E 1
#define DELETE_E 2
#define REMOVE_E 3
#define SEARCH_E 4

#define APPEND_STR_E "append"
#define DELETE_STR_E "delete"
#define REMOVE_STR_E "remove"
#define SEARCH_STR_E "search"

#define FAILURE_E 1
#define SUCCESS_E 2

// First ID is for client to server comms, second ID is for server responses.
#define RECIEVE_QUEUE_ID 1236
#define SEND_QUEUE_ID 1237

struct my_msg_st {
   long int my_msg_type;
   char some_text[BUFSIZ];
};

#endif // MESSAGE_QUEUE_CONSTS_H_INCLUDED
