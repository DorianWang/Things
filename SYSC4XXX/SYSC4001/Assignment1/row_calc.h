#ifndef ROW_CALC_H_INCLUDED
#define ROW_CALC_H_INCLUDED

#include <limits.h>

//https://stackoverflow.com/questions/14808908/pointer-to-2d-arrays-in-c
typedef int m_4x4_t[4][4];

// The program requirements state to multiply 4x4 matrices, so this program only supports 4x4.
typedef struct matrix_multiplication_4_by_4
{
   m_4x4_t* first;
   m_4x4_t* second;

   m_4x4_t* output;
} MMultStruct;




int calc_row_mmult(MMultStruct input_matrices);




#endif // ROW_CALC_H_INCLUDED
