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
   m_4x4_t M_T1 = {{1, 2, 4, 8}, {2, 4, -8, 16}, {4, 8, 16, 32}, {8, 16, 32, -64}};
   m_4x4_t N_T1 = {{2, 3, 5, 7}, {-2, 3, -5, 7}, {1, 5, 2, -9}, {0, -17, -2, 3}};
   m_4x4_t T1_RESULT = {{2, -107, -13, 9}, {-12, -294, -58, 162},
                     {8, -428, -52, 36}, {16, 1320, 152, -312}};

   // "Regular" matrix multiplication example
   m_4x4_t M_T2 = {{1, 2, 4, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}, {8, 16, 32, 64}};
   m_4x4_t N_T2 = {{2, 3, 5, 7}, {-2, 3, -5, 7}, {1, 5, 2, -9}, {0, -17, -2, 3}};
   m_4x4_t T2_RESULT = {{2, -107, -13, 9}, {4, -214, -26, 18},
                        {8, -428, -52, 36}, {16, -856, -104, 72}};

   // Identity matrix multiplication, should return the same value
   m_4x4_t M_T3 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {2, -4, 6, -8}, {-10, 12, -14, 16}};
   m_4x4_t I_4X4 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
   m_4x4_t* T3_RESULT = &M_T3;

   // Example Hadamard matrix, from wikipedia ("Walsh Matrix")
   m_4x4_t M_T4 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {2, -4, 6, -8}, {-10, 12, -14, 16}};
   m_4x4_t H_4X4 = {{1, 1, 1, 1}, {1, -1, 1, -1}, {1, 1, -1, -1}, {1, -1, -1, 1}};
   m_4x4_t T4_RESULT = {{10, -2, -4, 0}, {26, -2, -4, 0}, {-4, 20, 0, -8}, {4, -52, 0, 8}};

   // Example Rotation matrix
   m_4x4_t M_T5 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {2, -4, 6, -8}, {-10, 12, -14, 16}};
   m_4x4_t R_4X4 = {{-1, 0, 0, 0}, {0, -1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
   m_4x4_t T5_RESULT = {{-1, -2, 3, 4}, {-5, -6, 7, 8}, {-2, 4, 6, -8}, {10, -12, -14, 16}};

/*
   // Superseded by assemble_test_cases() below
   int common_operations_test_cases(MMultStruct** test_cases)
   {
      MMultStruct* values =
      (MMultStruct*) malloc(sizeof(MMultStruct) * 5);
      values[0].first = &M_T1; values[0].second = &N_T1; values[0].output = &T1_RESULT;
      values[1].first = &M_T2; values[1].second = &N_T2; values[1].output = &T2_RESULT;
      values[2].first = &M_T3; values[2].second = &I_4X4; values[2].output = T3_RESULT;
      values[3].first = &M_T4; values[3].second = &H_4X4; values[3].output = &T4_RESULT;
      values[4].first = &M_T5; values[4].second = &R_4X4; values[4].output = &T5_RESULT;
      *test_cases = values;

      return 5; // Total number of cases
   }
   */
#endif // TEST_CASES_COMMON_OPERATIONS


// Well, it should work, but it shouldn't exist \o_o/
int assemble_test_cases(MMultStruct** test_cases)
{
   int total_tests = 0;
   #ifndef DEACTIVATE_MANDATORY_TEST
      #ifdef TEST_CASES_COMMON_OPERATIONS
         total_tests = 6;
         MMultStruct* values = (MMultStruct*) malloc(sizeof(MMultStruct) * total_tests);
         values[0].first = &M; values[0].second = &N;
         values[1].first = &M_T1; values[1].second = &N_T1;
         values[2].first = &M_T2; values[2].second = &N_T2;
         values[3].first = &M_T3; values[3].second = &I_4X4;
         values[4].first = &M_T4; values[4].second = &H_4X4;
         values[5].first = &M_T5; values[5].second = &R_4X4;
         *test_cases = values;
      #else
         total_tests = 1;
         MMultStruct* values = (MMultStruct*) malloc(sizeof(MMultStruct) * total_tests);
         values[0].first = &M; values[0].second = &N;
         *test_cases = values;
      #endif
   #else
      #ifdef TEST_CASES_COMMON_OPERATIONS
         total_tests = 5;
         MMultStruct* values = (MMultStruct*) malloc(sizeof(MMultStruct) * total_tests);
         values[0].first = &M_T1; values[0].second = &N_T1; values[0].output = &T1_RESULT;
         values[1].first = &M_T2; values[1].second = &N_T2; values[1].output = &T2_RESULT;
         values[2].first = &M_T3; values[2].second = &I_4X4; values[2].output = T3_RESULT;
         values[3].first = &M_T4; values[3].second = &H_4X4; values[3].output = &T4_RESULT;
         values[4].first = &M_T5; values[4].second = &R_4X4; values[4].output = &T5_RESULT;
         *test_cases = values;
      #else
         total_tests = 0;
         *test_cases = NULL;
      #endif
   #endif
   return total_tests;
}















#endif // TEST_CASE_CONSTANTS_H_INCLUDED

