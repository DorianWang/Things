#ifndef TEST_CASE_CONSTANTS_H_INCLUDED
#define TEST_CASE_CONSTANTS_H_INCLUDED

#include "row_calc.h"

#ifndef DEACTIVATE_MANDATORY_TEST

   // Mandatory values hardcoded into the executable.
   m_4x4_t M = {{20, 20, 50, 40}, {10, 6, 70, 8},
                  {40, 3, 2, 10}, {8, 7, 6, 5}};
   m_4x4_t N = {{10, 30, 50, 70}, {1, 3, 6, 8},
                  {9, 5, 5, 7}, {8, 6, 4, 2}};

   // Expected values for mandatory values, entered from online calculator.
   m_4x4_t MN_RESULT = {{990, 1150, 1530, 1990}, {800, 716, 918, 1254},
                     {501, 1279, 2068, 2858}, {181, 321, 492, 668}};

#endif // DEACTIVATE_MANDATORY_TEST

#ifdef TEST_CASES_COMMON_OPERATIONS
   // Multiplication with mixed negatives
   m_4x4_t M_T1 = {{1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}, {8, 16, 32, 64}};
   m_4x4_t N_T1 = {{2, 3, 5, 7}, {-2, 3, -5, 7}, {1, 5, 2, -9}, {0, -17, -2, 3}};
   m_4x4_t T1_RESULT = {}; // TODO: enter values into a matrix calculator

   // "Regular" matrix multiplication example
   m_4x4_t M_T2 = {{1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}, {8, 16, 32, 64}};
   m_4x4_t N_T2 = {{2, 3, 5, 7}, {-2, 3, -5, 7}, {1, 5, 2, -9}, {0, -17, -2, 3}};

   m_4x4_t T2_RESULT = {};

   // Identity matrix multiplication, should return the same value
   m_4x4_t M_T3 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {2, -4, 6, -8}, {-10, 12, -14, 16}};
   m_4x4_t I_4X4 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
   m_4x4_t* T3_RESULT = &M_T3

   // Example Hadamard matrix, from wikipedia ("Walsh Matrix")
   m_4x4_t M_T4 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {2, -4, 6, -8}, {-10, 12, -14, 16}};
   m_4x4_t H_4X4 = {{1, 1, 1, 1}, {1, -1, 1, -1}, {1, 1, -1, -1}, {1, -1, -1, 1}};

   // Example Rotation matrix
   m_4x4_t M_T5 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {2, -4, 6, -8}, {-10, 12, -14, 16}};
   m_4x4_t R_4X4 = {{-1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};



#endif // TEST_CASES_COMMON_OPERATIONS



















#endif // TEST_CASE_CONSTANTS_H_INCLUDED

