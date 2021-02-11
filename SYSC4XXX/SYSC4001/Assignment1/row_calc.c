#include "row_calc.h"


/**
This function takes the row value, and three 4x4 int array pointers (first, second, output)
which are stored in a MMultStruct.

It returns the greatest value calculated in the row.
*/
int calc_row_mmult(int row_value, MMultStruct input_matrices)
{
   int highest_value = INT_MIN;
   int i; int j;
   for (i = 0; i < MATRIX_SIZE_CONST; i++){ // The Column value in the output matrix, row value is already defined.
      int temp_sum = 0;
      for (j = 0; j < MATRIX_SIZE_CONST; j++){ // Iterator for column value in first and row value in second.
         temp_sum += (*(input_matrices.first))[row_value][j] * (*(input_matrices.second))[j][i]; // This is very ugly but it works.
      }
      (*(input_matrices.output))[row_value][i] = temp_sum;
      if (temp_sum > highest_value){
         highest_value = temp_sum;
      }
   }

   return highest_value;
}

void print_m_4x4_t(m_4x4_t* input)
{
   int i;
   for (i = 0; i < 4; i++){
      printf("%8d %8d %8d %8d\n", (*(input))[i][0], (*(input))[i][1], (*(input))[i][2], (*(input))[i][3]);
   }
}

