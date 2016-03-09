/* main.c - SYSC 2006 Winter 2016 Lab 7 Test Harness 
 *
 * Tests revised March 2, 2016.
 */

#include <stdlib.h>  // exit, malloc, free
#include <stdio.h>   // printf
#include "sput.h"

#include "array_list.h"

/* By default, Pelles C generates "warning #2154: Unreachable code"
   and "warning #2130: Result of comparison is constant" when the 
   macros in sput.h are used. The following pragma directive disables the
   generation of these warnings.
 */
#pragma warn(disable: 2130 2154)

/* Returns true if the first n elements in arr1 are equal to the 
 * first n elements in arr2; otherwise returns false.
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

static void test_intlist_construct(void)
{
    intlist_t list = intlist_construct(10);
    sput_fail_unless(list.capacity == 10,
                     "list = intlist_construct(10),\n"
                     "Verifying list.capacity == 10");
    sput_fail_unless(list.size == 0,
                     "Verifying list.size == 0");
    sput_fail_unless(list.elems != NULL,
                     "Verifying list.elems is not NULL");

    intlist_destroy(list);
}

static void test_intlist_print(void)
{
    printf("\nTesting intlist_print\n\n");

    intlist_t list = intlist_construct(4);

    /* Print [] */

	list.size = 0;

    /* We can't assume that intlist_print is working, so we can't call it
     * to display the expected output!
     */
	printf("Expected output: []\n");
	printf("Actual output:   ");
	intlist_print(list);
	printf("\n\n");

    /* Print [1] */

	/* We can't assume intlist_append is working, so we have to initialize
	 * the list ourselves.
	 */
	list.elems[0] = 1;
	list.size = 1;

	printf("Expected output: [1]\n");
	printf("Actual output:   ");
	intlist_print(list);
	printf("\n\n");

    /* Print [1, 5, -3, 9] */

	list.elems[0] = 1;
	list.elems[1] = 5;
	list.elems[2] = -3;
	list.elems[3] = 9;
	list.size = 4;

	printf("Expected output: [1 5 -3 9]\n");
	printf("Actual output:   ");
	intlist_print(list);
	printf("\n\n");

    intlist_destroy(list);
}

static void test_intlist_append(void)
{
    int expected[] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    
    intlist_t list = intlist_construct(10);

    list = intlist_append(list, 0);

    sput_fail_unless(compare_arrays(list.elems, expected, 1), 
		             "list = intlist_construct(10);\n"
                     "list = intlist_append(list, 0);\n"
		             "Verifying list is now [0]");
    sput_fail_unless(list.size == 1,
                     "Verifying list size is 1");
    sput_fail_unless(list.capacity == 10,
                     "Verifying list capacity is 10");


    list = intlist_append(list, 2);

    sput_fail_unless(compare_arrays(list.elems, expected, 2), 
                     "list = intlist_append(list, 0);\n"
                     "Verifying list is now [0 2]");
    sput_fail_unless(list.size == 2,
                     "Verifying list size is 2");
    sput_fail_unless(list.capacity == 10,
                     "Verifying list capacity is 10");

    intlist_destroy(list);


	list = intlist_construct(10);
       
	for (int i = 0; i < 9; i++) {
        list = intlist_append(list, 2 * i);
    }
    /* The list is now [0, 2, 4, ..., 16], size == 9, capacity == 10 */

    list = intlist_append(list, 18);

    sput_fail_unless(compare_arrays(list.elems, expected, 10), 
                     "Initialized list to [0 2 4 6 8 10 12 14 16],\n"
                     "list = intlist_append(list, 18);\n"
                     "Verifying list is now [0 2 4 6 8 10 12 14 16 18]");
    sput_fail_unless(list.size == 10,
                     "Verifying list size is 10");
    sput_fail_unless(list.capacity == 10,
                     "Verifying list capacity is 10");


    /* Now attempt to append an integer to a full list.*/

    list = intlist_append(list, 20);

    sput_fail_unless(compare_arrays(list.elems, expected, 10), 
                     "list = intlist_append(list, 20);\n"
                     "Verifying list is unchanged: [0 2 4 6 8 10 12 14 16 18]");
    sput_fail_unless(list.size == 10,
                     "Verifying list size is 10");
    sput_fail_unless(list.capacity == 10,
                     "Verifying list capacity is 10");

    intlist_destroy(list);
}

static void test_intlist_capacity(void)
{
    intlist_t list = intlist_construct(10);

    sput_fail_unless(intlist_capacity(list) == 10, 
                     "list = intlist_construct(10),\n"
                     "Verifying intlist_capacity(list) returns 10");

    /* Initialize list to [0, 2, 4, 6, 8] */
    for (int i = 0; i < 5; i++) {
        list = intlist_append(list, 2 * i);
    }

    /* Verify that appending integers doesn't change the list's capacity. */
   
    sput_fail_unless(intlist_capacity(list) == 10, 
                     "Append 0, 2, 4, 6, 8,\n"
                     "Verifying intlist_capacity(list) returns 10");

    intlist_destroy(list);
}

static void test_intlist_size(void)
{   
    intlist_t list = intlist_construct(10);

    sput_fail_unless(intlist_size(list) == 0, 
                     "list = intlist_construct(10),\n"
                     "Verifying intlist_size(list) returns 0");

    /* Initialize list to [0, 2, 4, 6, 8] */
    for (int i = 0; i < 5; i++) {
        list = intlist_append(list, 2 * i);
    }

    sput_fail_unless(intlist_size(list) == 5, 
                     "Append 0, 2, 4, 6, 8,\n"
                     "Verifying intlist_size(list) returns 5");
  
    intlist_destroy(list);
}

static void test_intlist_get(void)
{
    intlist_t list = intlist_construct(10);

    /* Initialize list to [0, 2, 4, 6, 8] */
    for (int i = 0; i < 5; i++) {
        list = intlist_append(list, 2 * i);
    }

    sput_fail_unless(intlist_get(list, 0) == 0,
                     "list = intlist_construct(10),\n"
                     "Initialized list to [0 2 4 6 8],\n"
		             "Verifying intlist_get(list, 0) returns 0");

    sput_fail_unless(intlist_get(list, 1) == 2,
                     "Verifying intlist_get(list, 1) returns 2");

    sput_fail_unless(intlist_get(list, 2) == 4,
                     "Verifying intlist_get(list, 2) returns 4");

    sput_fail_unless(intlist_get(list, 3) == 6,
                     "Verifying intlist_get(list, 3) returns 6");

    sput_fail_unless(intlist_get(list, 4) == 8,
                     "Verifying intlist_get(list, 4) returns 8");

    intlist_destroy(list);
}

static void test_intlist_set(void)
{
    intlist_t list = intlist_construct(10);

    /* Initialize list to [0, 2, 4, 6, 8] */
    for (int i = 0; i < 5; i++) {
        list = intlist_append(list, 2 * i);
    }

    sput_fail_unless(intlist_set(list, 0, 1) == 0,
                    "list = intlist_construct(10),\n"
                     "Initialized list to [0 2 4 6 8],\n"
		             "Verifying intlist_set(list, 0, 1) returns 0");
    sput_fail_unless(intlist_get(list, 0) == 1,
                     "Verifying list element 0 is now 1");

    /* Verify that setting elements doesn't change the list's size or capacity. */
   
    sput_fail_unless(intlist_capacity(list) == 10, 
                     "Verifying intlist_capacity(list) returns 10");
    sput_fail_unless(intlist_size(list) == 5, 
                     "Verifying intlist_capacity(list) returns 5");

    sput_fail_unless(intlist_set(list, 1, 3) == 2,
                     "Verifying intlist_set(list, 1, 3) returns 2");
    sput_fail_unless(intlist_get(list, 1) == 3,
                     "Verifying list element 1 is now 3");

    sput_fail_unless(intlist_capacity(list) == 10, 
                     "Verifying intlist_capacity(list) returns 10");
    sput_fail_unless(intlist_size(list) == 5, 
                     "Verifying intlist_capacity(list) returns 5");

    sput_fail_unless(intlist_set(list, 2, 5) == 4,
                     "Verifying intlist_set(list, 2, 5) returns 4");
    sput_fail_unless(intlist_get(list, 2) == 5,
                     "Verifying list element 2 is now 5");

    sput_fail_unless(intlist_capacity(list) == 10, 
                     "Verifying intlist_capacity(list) returns 10");
    sput_fail_unless(intlist_size(list) == 5, 
                     "Verifying intlist_capacity(list) returns 5");

    sput_fail_unless(intlist_set(list, 3, 7) == 6,
                     "Verifying intlist_set(list, 3, 7) returns 6");
    sput_fail_unless(intlist_get(list, 3) == 7,
                     "Verifying list element 3 is now 7");

    sput_fail_unless(intlist_capacity(list) == 10, 
                     "Verifying intlist_capacity(list) returns 10");
    sput_fail_unless(intlist_size(list) == 5, 
                     "Verifying intlist_capacity(list) returns 5");

    sput_fail_unless(intlist_set(list, 4, 9) == 8,
                     "Verifying intlist_set(list, 4, 9) returns 8");
    sput_fail_unless(intlist_get(list, 4) == 9,
                     "Verifying list element 4 is now 9");

    sput_fail_unless(intlist_capacity(list) == 10, 
                     "Verifying intlist_capacity(list) returns 10");
    sput_fail_unless(intlist_size(list) == 5, 
                     "Verifying intlist_capacity(list) returns 5");

    intlist_destroy(list);
}

static void test_intlist_removeall(void)
{
    intlist_t list = intlist_construct(10);

    /* Initialize list to [0, 2, 4, 6, 8] */
    for (int i = 0; i < 5; i++) {
        list = intlist_append(list, 2 * i);
    }

    list = intlist_removeall(list);

    sput_fail_unless(intlist_size(list) == 0,
                     "list = intlist_construct(10),\n"
                     "Initialized list to [0 2 4 6 8],\n"
                     "list = intlist_removeall(list)\n"
                     "Verifying list size is now 0");

    sput_fail_unless(intlist_capacity(list) == 10,
                     "Verifying list capacity remains 10");

    sput_fail_unless(list.elems != NULL, 
                     "Verifying list.elems != NULL");

    intlist_destroy(list);
}

int main(void)
{
    sput_start_testing();

    sput_enter_suite("Exercise 1: intlist_construct()");
    sput_run_test(test_intlist_construct);

    test_intlist_print();

    sput_enter_suite("Exercise 3: intlist_append()");
    sput_run_test(test_intlist_append);

    sput_enter_suite("Exercise 4: intlist_capacity()");
    sput_run_test(test_intlist_capacity);

    sput_enter_suite("Exercise 5: intlist_size()");
    sput_run_test(test_intlist_size);

    sput_enter_suite("Exercise 6: intlist_get()");
    sput_run_test(test_intlist_get);

    sput_enter_suite("Exercise 7: intlist_set()");
    sput_run_test(test_intlist_set);

    sput_enter_suite("Exercise 8: intlist_removeall()");
    sput_run_test(test_intlist_removeall);

    sput_finish_testing();
    return sput_get_return_value();
}
