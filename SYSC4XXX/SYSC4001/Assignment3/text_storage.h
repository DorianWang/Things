#ifndef TEXT_STORAGE_H_INCLUDED
#define TEXT_STORAGE_H_INCLUDED
/**
   An objectively worse version of the C++ vector class, as an unordered array of string pointers.
   This is moderately simple in my opinion, but others may disagree.
   I am doing this mostly to avoid thinking about other assignments I'm failing.

   This structure dynamically resizes based on the number of stored objects,
   shuffling them when required, and also handles all the string operations.
**/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// The first two defines are for the maximum and minimum size of the sentence array.
// The array will not be grown or shrunk if the max size is equal or above/below the constants.
// If the number of free spaces is 3/4 times length, halve the size of the array.
// The array will attempt to double in size if it is full.
#define MAX_NUM_SENTENCES 512
#define MIN_NUM_SENTENCES 8

// Max number of characters (bytes) in a sentence, sentences larger will be discarded.
#define MAX_SENTENCE_LENGTH 64

// All whitespace characters in ascii, as well as some punctuation marks.
#define DELIMITERS " .,!?&/\\\t\r\n\v\f"

typedef struct text_storage
{
   size_t size;
   size_t max_size;
   char** internal_array;
} TextStore;

// Functions to create and delete the data structure.
TextStore* create_store();
void delete_store(TextStore* ts);

// Functions that act on the data structure.
int store_append(TextStore* ts, const char* str_in);
int store_delete(TextStore* ts, const char* str_in);
int store_remove(TextStore* ts, const char* str_in);
int store_remove_index(TextStore* ts, int index);
const char* store_search(TextStore* ts, const char* str_in);

// Functions which simplify internal code.
char* clean_search_string(const char* str_in);

// Testing and debugging functions
void DEBUG_PRINT(TextStore* ts);


#endif // TEXT_STORAGE_H_INCLUDED





