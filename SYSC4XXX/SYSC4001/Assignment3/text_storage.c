#include "text_storage.h"


TextStore* create_store()
{
   TextStore* ts = malloc(sizeof(TextStore));
   if (ts == NULL){
      perror("Failed to allocate text storage.");
      return NULL;
   }

   ts->internal_array = malloc(sizeof(char*) * MIN_NUM_SENTENCES);
   if (ts->internal_array == NULL){
      free(ts);
      perror("Failed to allocate memory required for text storage.");
      return NULL;
   }

   ts->max_size = MIN_NUM_SENTENCES;
   ts->size = 0;
   return ts;
}

void delete_store(TextStore* ts)
{
   if (ts != NULL){
      free(ts->internal_array);
      free(ts);
   }
}

int store_append(TextStore* ts, char* str_in)
{
   if (ts->size == ts->max_size){ // If previously out of memory, try again.
      if (ts->max_size >= MAX_NUM_SENTENCES){
         perror("Text storage is full and at max size, aborting append.");
         return 0;
      }
      void* new_array = realloc(ts->internal_array, (ts->max_size) * 2);
      if (new_array != NULL){
         ts->internal_array = new_array;
      }
      else{
         perror("Text storage is full and failed to expand, aborting append.");
         return 0;
      }
   }

   char* new_str;
   size_t new_len = strlen(str_in);
   if (new_len >= MAX_SENTENCE_LENGTH){
      perror("Sentence is too long, aborting append.");
      return 0;
   }

   new_str = malloc(sizeof(char) * (new_len + 1));
   if (new_str == NULL){
      perror("Failed to allocate sentence, aborting append.");
      return 0;
   }
   memcpy(new_str, str_in, new_len + 1);

   ts->internal_array[ts->size] = new_str;
   ts->size++;

   // If the array is full, try to expand it.
   if (ts->size == ts->max_size){
      if (ts->max_size < MAX_NUM_SENTENCES){
         void* new_array = realloc(ts->internal_array, (ts->max_size) * 2);
         if (new_array != NULL){
            ts->internal_array = new_array;
         }
      }
   }
   return 1;
}


int store_delete(TextStore* ts, char* str_in)
{
   /**
   This function has a lot of moving parts, so I'll explain what the variables are here.
   - sentence_iter is a pointer, pointing to the current last character checked.
   - sentence_length is an integer, equal to the number of characters in the sentence.
   - sentence_end is a pointer, pointing to the null terminator of the sentence.
   - search_str_length is an integer, equal to the number of characters in the search string.

   The algorithm searches the sentence for the search string. It then checks if the characters
   surrounding the word are not alphanumeric characters. If so, the sentence is zeroed for the
   length of the search string (marking it for deletion). The sentence iterator is then set to
   the end of the found substring. When the sentence iterator is either null or set to the end
   of the sentence, stop looping and then take all non-null characters in the sentence to make
   a new sentence, which is then written back to the sentence array. This code will skip where
   appropriate.
   **/
   char buffer[MAX_SENTENCE_LENGTH];
   for(size_t i; i < ts->size; i++){
      char found = 0;
      char* sentence_iter = ts->internal_array[i];
      size_t sentence_length = strlen(ts->internal_array[i]);
      char* sentence_end = ts->internal_array[i] + sentence_length;
      size_t search_str_length = strlen(str_in);
      if (search_str_length == 0){
         return 1; // Can't search for nothing, but not an error.
      }

      while(sentence_iter < sentence_length){
         sentence_iter = strstr(sentence_iter, str_in);
         if (sentence_iter == NULL){ // Not found in the rest of the array.
            break;
         }
         if (sentence_iter == ts->internal_array[i]){
            //TODO: Finish up eraser.
            (sentence_iter + 2)
         }
         found = 1;
      }
      if (found){
         size_t counter = 0;
         for(size_t j; j < sentence_length; j++){
            if (internal_array[i][j] != '\0'){
               buffer[counter] = ts->internal_array[i][j];
               counter++;
            }
         }
         buffer[counter++] = '\0'; // Add back null terminator which was removed in the loop
         memcpy(ts->internal_array[i], buffer, counter); // Set sentence to new sentence.
      } // If nothing was found then just go to the next sentence in the list.
   }
}

int store_remove(TextStore* ts, char* str_in);
char* store_search(TextStore* ts, char* str_in);













