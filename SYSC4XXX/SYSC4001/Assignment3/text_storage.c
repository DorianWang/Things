#include "text_storage.h"

// Creates a new string from the input. It is a lowercase word without additional spaces.
// Returns NULL if the resultant string would be empty, contains a delimiter, or malloc fails.
// It is the callers responsibility to free the returned string.
// My testing shows it works for all intended inputs
char* clean_search_string(const char* str_in)
{
   size_t in_len = strlen(str_in);
   size_t i;
   size_t str_start = 0;
   size_t str_end = 0;
   if (in_len == 0){
      // Can't search for nothing, but that isn't an error I guess.
      printf("First!\n");
      return NULL;
   }
   str_start = strspn(str_in, DELIMITERS); // Gets the length of delimiters from the start;
   if (str_start == in_len){ // The input string is functionally empty.
      printf("Second!\n");
      return NULL;
   }
   for (i = in_len - 1; i > str_start; i--){
      if (strchr(DELIMITERS, str_in[i]) == NULL){ // A non-whitespace character was found.
         if (str_end == 0){ // An end was found.
            str_end = i + 1;
         }
      }
      else{
         if (str_end != 0){ // An end was already found, input is not valid.
            printf("Third!\n");
            return NULL;
         }
      }
   }
   size_t new_len = str_end - str_start;
   char* new_str = malloc(sizeof(char) * (1 + new_len));

   if (new_str == NULL){
      perror("Failed to allocate new string, aborting search.");
      return NULL;
   }
   for (i = 0; i < new_len; i++){
      new_str[i] = tolower(str_in[str_start + i]);
   }
   new_str[new_len] = '\0';
   return new_str;
}


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
      size_t i;
      for(i = 0; i < ts->size; i++){
         free(ts->internal_array[i]);
      }
      free(ts->internal_array);
      free(ts);
   }
}

int store_append(TextStore* ts, const char* str_in)
{
   if (ts == NULL){
      perror("Pointer to TextStore is NULL, aborting append.");
      return 0;
   }

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
         void* new_array = realloc(ts->internal_array, sizeof(char*) * (ts->max_size) * 2);
         if (new_array != NULL){
            ts->internal_array = new_array;
            ts->max_size = ts->max_size * 2;
         }
      }
   }
   return 1;
}

int store_delete(TextStore* ts, const char* str_in)
{
   /**
   This function tokenizes a lowercase copy of each sentence, and then compares it with the input.
   It then puts the sentence back together with the delimiters.
   Any input strings larger than MAX_SENTENCE_LENGTH will be truncated.
   The function will strip all leading and trailing delimiters in the input, and will not search
   if the input string contains a delimiter inside it.

   Returns 1 if successful, or 0 otherwise.
   **/
   char buffer[MAX_SENTENCE_LENGTH];
   char* mark_deletion;
   char* new_search_str = clean_search_string(str_in);
   size_t search_str_length;
   size_t i; size_t j;

   if (ts->size == 0){
      return 1; // Can't delete from nothing, but isn't an error.
   }
   else{
      mark_deletion = calloc(ts->size, sizeof(char));
   }

   if (ts == NULL){
      perror("Pointer to TextStore is NULL, aborting delete.");
      return 0;
   }

   if (new_search_str == NULL){
      return 0; // Failed to get a clean string.
   }
   else{
      search_str_length = strlen(new_search_str);
   }

   for(i = 0; i < ts->size; i++){
      size_t old_sentence_length = strlen(ts->internal_array[i]);
      char* token;
      size_t new_sentence_counter;
      size_t last_removed_token_end;
      char has_words; // This tracks to see if there is anything left in the sentence after.
      memcpy(buffer, ts->internal_array[i], old_sentence_length + 1);
      for (j = 0; j < old_sentence_length; j++){
         buffer[j] = tolower(buffer[j]); // Make sentence copy lowercase.
      }
      token = strtok(buffer, DELIMITERS);
      last_removed_token_end = 0; new_sentence_counter = 0; has_words = 0;

      while (token != NULL){
         if (strcmp(token, new_search_str) == 0){ // Word Found!
            size_t buff_loc = (token - buffer); // Index of where the token was found.
            size_t chunk_size = buff_loc - last_removed_token_end;
            if (chunk_size > 0){
               memmove(ts->internal_array[i] + new_sentence_counter,
                      ts->internal_array[i] + last_removed_token_end, chunk_size);
            }
            last_removed_token_end = buff_loc + search_str_length;
            new_sentence_counter += chunk_size;
         }
         else
         {
            has_words++; // A word was found, which should not be deleted.
         }
         token = strtok(NULL, DELIMITERS);
      }
      //printf("Values: %llu %llu\n", new_sentence_counter, last_removed_token_end);

      if (has_words == 0){ // The sentence has no words, delete it later.
         mark_deletion[i] = 1;
      }

      // Add the last part of the sentence back if required.
      if (last_removed_token_end != old_sentence_length){
         memmove(ts->internal_array[i] + new_sentence_counter, ts->internal_array[i] +
                last_removed_token_end, old_sentence_length - last_removed_token_end);
         new_sentence_counter += old_sentence_length - last_removed_token_end;
      }
      ts->internal_array[i][new_sentence_counter] = '\0'; // Add null terminator to end.

   }

   for (i = ts->size; i > 0; i--){
      if(mark_deletion[i - 1]){
         store_remove_index(ts, i - 1);
      }
   }
   free(mark_deletion);
   free(new_search_str);
   return 1;
}

// Removes the first matching sentence, then shifts the rest to the left to fill the gap.
// Maintains order, required in this assignment. Otherwise I would just swap in the last value.
int store_remove(TextStore* ts, const char* str_in)
{
   char shifting = 0;
   size_t i;
   if (ts == NULL){
      perror("Pointer to TextStore is NULL, aborting remove.");
      return 0;
   }

   for(i = 0; i < ts->size; i++){
      // printf("%s | L: %llu | %s | L: %llu\r\n", ts->internal_array[i], strlen(ts->internal_array[i]), str_in, strlen(str_in));
      if (shifting){
         ts->internal_array[i - 1] = ts->internal_array[i];
      }
      else if (strcmp(ts->internal_array[i], str_in) == 0){
         //printf("They compared equal!\n");
         free(ts->internal_array[i]);
         shifting = 1;
      }
   }
   if (shifting){
      (ts->size)--;

      if (ts->max_size > MIN_NUM_SENTENCES && ts->size <= (ts->max_size / 4)){
         void* new_array = realloc(ts->internal_array, sizeof(char*) * (ts->max_size) / 2);
         if (new_array != NULL){
            ts->internal_array = new_array;
            ts->max_size = ts->max_size / 2;
         }
      }

      return 1;
   }
   return 0;
}

// Removes the specific sentence by index, could be unsafe if iterating by size.
// Only used inside this file, to clean up wordless sentences.
int store_remove_index(TextStore* ts, int index)
{
   size_t i; char shifting = 0;
   if (ts == NULL || index >= ts->size){
      perror("Something is wrong with index removal!");
      return 0;
   }

   for(i = 0; i < ts->size; i++){
      if (shifting){
         ts->internal_array[i - 1] = ts->internal_array[i];
      }
      else if (i == index){
         free(ts->internal_array[i]);
         shifting = 1;
      }
   }

   (ts->size)--;
   if (ts->max_size > MIN_NUM_SENTENCES && ts->size <= (ts->max_size / 4)){
      void* new_array = realloc(ts->internal_array, sizeof(char*) * (ts->max_size) / 2);
      if (new_array != NULL){
         ts->internal_array = new_array;
         ts->max_size = ts->max_size / 2;
      }
   }
   return 1;

}

// Return the first sentence found which contains the word, or NULL if none was found.
const char* store_search(TextStore* ts, const char* str_in)
{
   char buffer[MAX_SENTENCE_LENGTH];
   size_t i; size_t j; char* token;
   char* new_search_str = clean_search_string(str_in);
   size_t sentence_len;

   if (new_search_str == NULL){
      return NULL; // Invalid search string.
   }
   if (ts == NULL){
      perror("Pointer to TextStore is NULL, aborting append.");
      free(new_search_str);
      return NULL;
   }


   for(i = 0; i < ts->size; i++){
      sentence_len = strlen(ts->internal_array[i]);
      memcpy(buffer, ts->internal_array[i], sentence_len);
      for(j = 0; j < sentence_len; j++){
         buffer[j] = tolower(buffer[j]);
      }
      token = strtok(buffer, DELIMITERS);
      while(token != NULL){
         if (strcmp(token, new_search_str) == 0){ // Word found, return!
            free(new_search_str);
            return ts->internal_array[i];
         }
         token = strtok(NULL, DELIMITERS);
      }
   }
   free(new_search_str);
   return NULL;
}


void DEBUG_PRINT(TextStore* ts)
{
   size_t i;
   printf("The max size is %lu and the current size is %lu.\r\n", ts->max_size, ts->size);
   printf("The stored lines are:\n");
   for (i = 0; i < ts->size; i++){
      printf("%s | Length: %lu\r\n", ts->internal_array[i], strlen(ts->internal_array[i]));
   }
}










