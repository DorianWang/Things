/* 
 * SYSC 2006 Winter 2016 Lab 7
 */

#include <stdbool.h>

typedef struct {
    int     *elems;
    int     capacity;   /* Maximum number of elements in the list. */
    int     size; 		/* Current number of elements in the list */
} intlist_t;

intlist_t intlist_construct(int capacity);
intlist_t intlist_destroy(intlist_t list);

int intlist_size(const intlist_t list);
int intlist_capacity(const intlist_t list);

intlist_t intlist_append(intlist_t list, int element);
int intlist_get(const intlist_t list, int index);
int intlist_set(intlist_t list, int index, int element);
intlist_t intlist_removeall(intlist_t list);

void    intlist_print(const intlist_t list);
