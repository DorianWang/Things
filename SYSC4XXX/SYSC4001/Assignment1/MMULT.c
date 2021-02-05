#include <stdio.h>
#include <stdlib.h>

#include "row_calc.h"
#include "test_case_constants.h"



int main()
{
   m4x4_type x = {{1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}, {8, 16, 32, 64}};
   m4x4_type* y = &x;
   printf("%d\n", sizeof(MMultStruct));
   printf("Hello world!\n");
   return 0;
}
