#ifndef ROW_CALC_H_INCLUDED
#define ROW_CALC_H_INCLUDED

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE_CONST 4

// https://stackoverflow.com/questions/14808908/pointer-to-2d-arrays-in-c
// First value is row, second is column
typedef int m_4x4_t[MATRIX_SIZE_CONST][MATRIX_SIZE_CONST];

// The program requirements state to multiply 4x4 matrices, so this program only supports 4x4.
typedef struct matrix_multiplication_4_by_4
{
   m_4x4_t* first;
   m_4x4_t* second;

   m_4x4_t* output;
} MMultStruct;




int calc_row_mmult(int row_value, MMultStruct input_matrices);
void print_m_4x4_t(m_4x4_t* input);



#endif // ROW_CALC_H_INCLUDED
