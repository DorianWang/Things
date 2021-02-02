#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
   printf("The value of argc is: %d\n", argc);

   int i;
   for (i = 0; i < argc; i++){
      // This keeps the first argument ("environment?"), which may not be desired.
      printf(argv[i]);
      printf("\n");
   }
}



