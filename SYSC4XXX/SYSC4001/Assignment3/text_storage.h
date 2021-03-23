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

typedef struct text_storage
{
   size_t size;
   size_t max_size;
   char** internal_array;
} TextStore;


TextStore* create_store();
void delete_store(TextStore* ts);

int store_append(TextStore* ts, char* str_in);
int store_delete(TextStore* ts, char* str_in);
int store_remove(TextStore* ts, char* str_in);
char* store_search(TextStore* ts, char* str_in);


#endif // TEXT_STORAGE_H_INCLUDED





