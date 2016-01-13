#include "FileIO_C.h"


unsigned int readFileText(FILE* file, char** outputLines, unsigned int numLines, int bufferSize){
   int i = 0;
   if (bufferSize != NULL){
      char* a = (char*) malloc(sizeof(char) * numLines * bufferSize);
      if (a){
         for (i = 0; i < numLines; i++){
            outputLines[i] = &(a[i * bufferSize]);
         }
      }
      else{
         *outputLines = NULL;
         return 0; // Memory failure.
      }
   }
   else{
      char* a = malloc(sizeof(char) * numLines * DEFAULT_MAX_READ);
      if (a){
         for (i = 0; i < numLines; i++){
            outputLines[i] = &(a[i * DEFAULT_MAX_READ]);
         }
      }
      else{
         *outputLines = NULL;
         return 0; // Memory failure.
      }
   }
   i = 0;
   for (i = 0; i < numLines; i++){
         //TODO: Things from here.
      char *fgets( char *buf, int n, FILE *fp );
   }

return 1;
}

unsigned int readFileData(FILE* File, char**, unsigned int maxRead){
   if (maxRead){
      char* buffer = malloc(sizeof(char) * maxRead);
   }
   else{
      char* buffer = malloc(sizeof(char) * DEFAULT_MAX_READ);
   }


   fread(void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);


}









const struct library FileIO_C = {
    .readFileText = readFileText,
    .readFileData = readFileData,
};






