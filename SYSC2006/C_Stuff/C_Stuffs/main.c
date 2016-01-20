#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileIO_C.h"

void allFileLines(){
   FILE* inputFile;
};









int main()
{
   //printf("Hello world!\n");

   char* buffer;


   FILE* File = fopen("DataTest.txt", "rb");
   FileIO_C.readFileData(File, &buffer, 4);

   buffer[4] = NULL;

   printf(buffer);
   printf("Oui Oui Oui!\n");



   return 0;
}















