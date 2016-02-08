#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FileIO_C.h"

#define endl "\n"

void allFileLines(){
   FILE* inputFile;
};




int main()
{
   //printf("Hello world!\n");

   char* buffer;
   char** twoBuffer;


   FILE* File = fopen("DataTest.txt", "rb");
   FileIO_C.readFileData(File, &buffer, 4);

   FILE* File2 = fopen("TextTest.txt", "r");
   FileIO_C.readFileText(File, &twoBuffer, 4, 0);

   buffer[4] = 0;

   system("PAUSE");

   printf(buffer);
   printf(endl);
   printf("Oui Oui Oui!\n");
   free(buffer);

   system("PAUSE");
   //printf(twoBuffer[0]);
   //printf(twoBuffer[1]);
   //printf(twoBuffer[2]);
   //printf(twoBuffer[3]);

   return (EXIT_SUCCESS);
}















