/* SYSC 2006 Winter 2016 Lab 4 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "sput.h"
/* By default, Pelles C generates "warning #2154: Unreachable code"
   and "warning #2130: Result of comparison is constant" when the
   macros in sput.h are used. The following pragma directive disables the
   generation of these warnings.
 */
#pragma warn(disable: 2130 2154)

/* ----  Do not change any of the statements above this line. --------- */

//I can change this into something different if required.
void swapInt(int arr[], int a, int b){
   arr[a] = arr[a] ^ arr[b];
   arr[b] = arr[a] ^ arr[b];
   arr[a] = arr[a] ^ arr[b];
}

/* Exercise 1 */
//How to spagetti!
void print_array(int arr[], int n)
{
   int i;
   printf("{");
   for (i = 0; i < n; i ++){
      printf("%i, ", arr[i]);
   }
   printf("\b\b}");
   //printf("This function should print the first n elements of arr\n");
}

/* Exercise 2. */

void reverse(int arr[], int n)
{
   int i;
   for (i = 0; i < n/2; i ++){
      swapInt(arr, i, n - i - 1);
   }
}

/* Exercise 3. */

void rotate_left(int arr[], int n)
{
   int i;
   for (i = 0; i < n - 1; i++){
      swapInt(arr, i, i + 1);
   }
}

/* Exercise 4. */

void ten_run(int arr[], int n)
{
   int i = 0;
   int tempTen = -1;
   while (i < n){
      if (arr[i] % 10){
         if (tempTen != -1){
            arr[i] = tempTen;
         }
      }
      else
      {
         tempTen = arr[i];
      }
      i++;
   }
}

/* Exercise 5. */
// Trigger warning, bad code ahead!
void without_tens(int arr[], int n)
{
   int i = n - 1;
   int n2 = n;
   while (i >= 0){
      if (arr[i] == 10){ // Bubblesort?
         swapInt(arr, i, i + 1);
         continue;
      }
      i++;
   }
   for (n = n2 - 1; n >= 0; n++)
      if (arr[n - 1] == 10){
         arr[n - 1] = 0;
         n--;
         continue;
      }
      break;
   }
}


/*---------------------------------------------------------------------------
 * Test harness for this lab. Do not modify any of the code below this line.
 */


static void test_print_array(void)
{
    int a1[] = {1, 2, 3, 4};
    printf("Calling print_array to print {1, 2, 3, 4}; verify output\n");
    print_array(a1,4);
    printf("\n");

    int a2[] = {1};
    printf("Calling print_array to print {1}; verify output\n");
    print_array(a2,1);
    printf("\n");
}

/* Return true if the first n elements in arrays arr1 and arr2
 * are equal, otherwise return false.
 */
static _Bool compare_arrays(int arr1[], int arr2[], int n)
{
    for (int i = 0; i < n; i = i + 1) {
        if (arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

static void test_reverse(void)
{
    int a1[] = {1, 2, 3, 4, 5, 6};
    int expected1[] = {6, 5, 4, 3, 2, 1};

    int a2[] = {1, 2, 3, 4, 5, 6, 7};
    int expected2[] = {7, 6, 5, 4, 3, 2, 1};

    int a3[] = {1, 2};
    int expected3[] = {2, 1};

    int a4[] = {1};
    int expected4[] = {1};

    reverse(a1, 6);
    sput_fail_unless(compare_arrays(a1, expected1, 6),
                     "\nreverse({1, 2, 3, 4, 5, 6}, 6) result is: "
                     "{6, 5, 4, 3, 2, 1}");

    reverse(a2, 7);
    sput_fail_unless(compare_arrays(a2, expected2, 7),
                     "\nreverse({1, 2, 3, 4, 5, 6, 7}, 7) result is: "
                     "{7, 6, 5, 4, 3, 2, 1}");

    reverse(a3, 2);
    sput_fail_unless(compare_arrays(a3, expected3, 2),
                     "\nreverse({1, 2}, 2) result is: "
                     "{2, 1}");

    reverse(a4, 1);
    sput_fail_unless(compare_arrays(a4, expected4, 1),
                     "\nreverse({1}, 1) result is: "
                     "{1}");
}

static void test_rotate_left(void)
{
    int a1[] = {6, 2, 5, 3};
    int expected1[] = {2, 5, 3, 6};

    int a2[] = {1, 2};
    int expected2[] = {2, 1};

    int a3[] = {1};
    int expected3[] = {1};

    int a4[] = {1, 1, 2, 2, 4};
    int expected4[] = {1, 2, 2, 4, 1};

    int a5[] = {1, 1, 1};
    int expected5[] = {1, 1, 1};

    int a6[] = {1, 2, 3};
    int expected6[] = {2, 3, 1};

    rotate_left(a1, 4);
    sput_fail_unless(compare_arrays(a1, expected1, 4),
                     "\nrotate_left({6, 2, 5, 3}, 4) result is: "
                     "{2, 5, 3, 6}");

    rotate_left(a2, 2);
    sput_fail_unless(compare_arrays(a2, expected2, 2),
                     "\nrotate_left({1, 2}, 2) result is: "
                     "{2, 1}");

    rotate_left(a3, 1);
    sput_fail_unless(compare_arrays(a3, expected3, 1),
                     "\nrotate_left({1}, 1) result is: "
                     "{1}");

    rotate_left(a4, 5);
    sput_fail_unless(compare_arrays(a4, expected4, 5),
                     "\nrotate_left({1, 1, 2, 2, 4}, 5) result is: "
                     "{1, 2, 2, 4, 1}");

    rotate_left(a5, 3);
    sput_fail_unless(compare_arrays(a5, expected5, 3),
                     "\nrotate_left({1, 1, 1}, 3) result is: "
                     "{1, 1, 1}");

    rotate_left(a6, 3);
    sput_fail_unless(compare_arrays(a6, expected6, 1),
                     "\nrotate_left({1, 2, 3}, 3) result is: "
                     "{2, 3, 1}");
}

static void test_ten_run(void)
{
    int a1[] = {2, 10, 3, 4, 20, 5};
    int expected1[] = {2, 10, 10, 10, 20, 20};

    ten_run(a1, 6);
    sput_fail_unless(compare_arrays(a1, expected1, 6),
                     "\nten_run({2, 10, 3, 4, 20, 5}, 6) result is: "
                     "{2, 10, 10, 10, 20, 20}");

    int a2[] = {10, 1, 20, 2};
    int expected2[] = {10, 10, 20, 20};

    ten_run(a2, 4);
    sput_fail_unless(compare_arrays(a2, expected2, 4),
                     "\nten_run({10, 1, 20, 2}, 4) result is: "
                     "{10, 10, 20, 20}");

    int a3[] = {10, 1, 9, 20};
    int expected3[] = {10, 10, 10, 20};

    ten_run(a3, 4);
    sput_fail_unless(compare_arrays(a3, expected3, 4),
                     "\nten_run({10, 1, 9, 20}, 4) result is: "
                     "{10, 10, 10, 20}");

    int a4[] = {1, 2, 50, 1};
    int expected4[] = {1, 2, 50, 50};

    ten_run(a4, 4);
    sput_fail_unless(compare_arrays(a4, expected4, 4),
                     "\nten_run({1, 2, 50, 1}, 4) result is: "
                     "{1, 2, 50, 50}");

    int a5[] = {1, 20, 50, 1};
    int expected5[] = {1, 20, 50, 50};

    ten_run(a5, 4);
    sput_fail_unless(compare_arrays(a5, expected5, 4),
                     "\nten_run({1, 20, 50, 1}, 4) result is: "
                     "{1, 20, 50, 50}");

    int a6[] = {10, 10};
    int expected6[] = {10, 10};

    ten_run(a6, 2);
    sput_fail_unless(compare_arrays(a6, expected6, 2),
                     "\nten_run({10, 10}, 2) result is: "
                     "{10, 10}");

    int a7[] = {10, 2};
    int expected7[] = {10, 10};

    ten_run(a7, 2);
    sput_fail_unless(compare_arrays(a7, expected7, 2),
                     "\nten_run({10, 2}, 2) result is: "
                     "{10, 10}");

    int a8[] = {0, 2};
    int expected8[] = {0, 0};

    ten_run(a8, 2);
    sput_fail_unless(compare_arrays(a8, expected8, 2),
                     "\nten_run({0, 2}, 2) result is: "
                     "{0, 0}");

    int a9[] = {1, 2};
    int expected9[] = {1, 2};

    ten_run(a9, 2);
    sput_fail_unless(compare_arrays(a9, expected9, 2),
                     "\nten_run({1, 2}, 2) result is: "
                     "{1, 2}");

    int a10[] = {1};
    int expected10[] = {1};

    ten_run(a10, 1);
    sput_fail_unless(compare_arrays(a10, expected10, 1),
                     "\nten_run({1}, 1) result is: "
                     "{1}");

}

void test_without_tens(void)
{
    int a1[] = {1, 10, 10, 2};
    int expected1[] = {1, 2, 0, 0};

    without_tens(a1, 4);
    sput_fail_unless(compare_arrays(a1, expected1, 4),
                     "\nwithout_tens({1, 10, 10, 2}, 4) result is: "
                     "{1, 2, 0, 0}");

    int a2[] = {10, 2, 10};
    int expected2[] = {2, 0, 0};

    without_tens(a2, 3);
    sput_fail_unless(compare_arrays(a2, expected2, 3),
                     "\nwithout_tens({10, 2, 10}, 3) result is: "
                     "{2, 0, 0}");

    int a3[] = {1, 99, 10};
    int expected3[] = {1, 99, 0};

    without_tens(a3, 3);
    sput_fail_unless(compare_arrays(a3, expected3, 3),
                     "\nwithout_tens({1, 99, 10}, 3) result is: "
                     "{1, 99, 0}");

    int a4[] = {10, 13, 10, 14};
    int expected4[] = {13, 14, 0, 0};

    without_tens(a4, 4);
    sput_fail_unless(compare_arrays(a4, expected4, 4),
                     "\nwithout_tens({10, 13, 10, 14}, 4) result is: "
                     "{13, 14, 0, 0}");

    int a5[] = {10, 13, 10, 14, 10};
    int expected5[] = {13, 14, 0, 0, 0};

    without_tens(a5, 5);
    sput_fail_unless(compare_arrays(a5, expected5, 5),
                     "\nwithout_tens({10, 13, 10, 14, 10}, 5) result is: "
                     "{13, 14, 0, 0, 0}");

    int a6[] = {10, 10, 3};
    int expected6[] = {3, 0, 0};

    without_tens(a6, 3);
    sput_fail_unless(compare_arrays(a6, expected6, 3),
                     "\nwithout_tens({10, 10, 3}, 3) result is: "
                     "{3, 0, 0}");

    int a7[] = {1};
    int expected7[] = {1};

    without_tens(a7, 1);
    sput_fail_unless(compare_arrays(a7, expected7, 1),
                     "\nwithout_tens({1}, 1) result is: "
                     "{1}");

    int a8[] = {13, 1};
    int expected8[] = {13, 1};

    without_tens(a8, 2);
    sput_fail_unless(compare_arrays(a8, expected8, 2),
                     "\nwithout_tens({13, 1}, 2) result is: "
                     "{13, 1}");

    int a9[] = {10};
    int expected9[] = {0};

    without_tens(a9, 1);
    sput_fail_unless(compare_arrays(a9, expected9, 1),
                     "\nwithout_tens({10}, 1) result is: "
                     "{0}");

}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("Exercise 1: print_array()");
    sput_run_test(test_print_array);

    sput_enter_suite("Exercise 2: reverse()");
    sput_run_test(test_reverse);

    sput_enter_suite("Exercise 3: rotate_left()");
    sput_run_test(test_rotate_left);

    sput_enter_suite("Exercise 4: ten_run()");
    sput_run_test(test_ten_run);

    sput_enter_suite("Exercise 5: without_tens()");
    sput_run_test(test_without_tens);

    sput_finish_testing();
    return sput_get_return_value();
}
