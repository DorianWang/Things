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
 * If the list is full, double the list capacity before inserting the element.
 */
intlist_t intlist_append(intlist_t list, int elem)
{
   if (list.size >= list.capacity){
      increase_capacity(list, list.capacity * 2);
   }

   list.elems[list.size] = elem;
   list.size++;
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

/*Return the index (position) of the first occurrence of an integer
*	equal to target in the specified list.
*	Returns -1 if target is not in the list. */

int intlist_index(const intlist_t list, int target)

{
   int i = 0;
   for (i = 0; i < list.size; i++){
      if (list.elems[i] == target){
         return i;
      }
   }
   return -1;

}

/*Count the number of integers that are equal to target int he
* specified list, and return that number.
*/

int intlist_count(const intlist_t list,int target)

{
   int i = 0;
   int intCount = 0;
   for (i = 0; i < list.size; i++){
      if (list.elems[i] == target){
         intCount++;
      }
   }
   return intCount;
}



/*Determine if an integer in the specified list is equal to target.
* Return true if target is in the list, otherwise return false.
*/

_Bool intlist_contains(const intlist_t list, int target)

{
   if (intlist_index(list, target) != -1){
      return true;
   }
   return false;
}

/*Increase the capacity of the specified list to new_capacity, and
*	return the list.

*	Example: suppose a list passed to increase_capacity has capacity 5
*	and contains [1,2,3,4,5]; i.e., its size is 5.If new_capacity
*	is 10,the list returned by this function will contain
*	[1,2,3,4,5], so the list size will be 5, but its capacity will
*	be 10.

*	Terminate the program via assert if new_capacity is not greater
*	than the list's current capacity.
*/

intlist_t increase_capacity(intlist_t list, int new_capacity)

{
   assert(new_capacity > list.capacity);
   int* newList = malloc(sizeof(int) * new_capacity);
   assert(newList != NULL);
   int i = 0;
   for (i = 0; i < list.size; i++){
      newList[i] = list.elems[i];
   }
   list.elems = newList;
   list.capacity = new_capacity;

   return list;

}

/*Delete the integer at the specified position in the specified
*	list, and return the modified list.
*	Parameter index is the position of the integer that should be
*	removed.

*	Terminate the program via assert if index is not in the range
*	0..intlist_size()-1.
*/

intlist_t intlist_delete(intlist_t list, int index)

{
   assert(index >= 0 && index < list.size);
   int i;
   for (i = index; i < list.size - 1; i++){
      list.elems[i] = list.elems[i + 1];
   }
   list.size--;
   return list;

}













