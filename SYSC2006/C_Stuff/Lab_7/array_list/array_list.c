/* SYSC 2006 Winter 2016 Lab 7
 *
 * array_list_soln.c
 *
 * The module implements a dynamically allocated, fixed-capacity list.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "array_list.h"

/* Construct a new, empty list, and return it.
 * Parameter capacity is the capacity of the backing array;
 * i.e., the # of elements that can be stored in the list.
 * Terminate the program via assert if capacity is <= 0.
 * Terminate the program via assert if memory for the list's
 * backing array cannot be allocated from the heap.
 */
intlist_t intlist_construct(int capacity)
{
    // Note - this is an incomplete function definition.
    // You must modify the function so that it correctly implements all
    // of the requirements listed in the lab handout and the
    // header comment.

    intlist_t list;
    assert(capacity > 0);

    list.elems = malloc(sizeof(int) * capacity);

    assert(list.elems != NULL);
    list.capacity = capacity;
    list.size = 0;

    return list;
}

/* Destroy the specified list: deallocate the backing array, and
 * set the list's size and capacity to 0.
 */
intlist_t intlist_destroy(intlist_t list)
{
    free(list.elems);   // Return the backing array to the heap.
    list.size = 0;
    list.capacity = 0;
    return list;
}

/* Print the specified list on the console.
 */
void intlist_print(const intlist_t list)
{
   int i = 0;
   printf("[");
   for (i = 0; i < list.size; i++){
      printf("%i ", list.elems[i]);
   }
   if (list.size) {printf("\b");}
   printf("]\n");
   //printf("intlist_print not yet implemented\n");
}

/* Insert element at the end of the specified list, and return the list.
 * If the list is full, element is not inserted and the unmodified list
 * is returned.
 */
intlist_t intlist_append(intlist_t list, int elem)
{
   if (list.size < list.capacity){
      list.elems[list.size] = elem;
      list.size++;
   }
   return list;
}

/* Return the maximum number of integers that can be stored in the
 * specified list.
 */
int intlist_capacity(const intlist_t list)
{
    return list.capacity;
}

/* Return the number of integers in the specified list.
 */
int intlist_size(const intlist_t list)
{
    return list.size;
}

/* Return the list element located at the specified index.
 * Terminate the program via assert if index is not in the range
 * 0 .. intlist_size() - 1.
 */
int intlist_get(const intlist_t list, int index)
{
   assert(index < list.size && index >= 0);
   return list.elems[index];
}

/* Store the specified element at the specified index in the list.
 * Return the integer that was previously stored at that index.
 * Terminate the program via assert if index is not in the range
 * 0 .. intlist_size() - 1.
 */
int intlist_set(intlist_t list, int index, int elem)
{
   assert(index < list.size && index >= 0);
   int temp = list.elems[index];
   list.elems[index] = elem;
   return temp;
}

/* Empty the specified list and return it.
 * Memory allocated to the list's backing array is not freed, so the
 * emptied list can continue to be used.
 */
intlist_t intlist_removeall(intlist_t list)
{
   list.size = 0;
   return list;
}
