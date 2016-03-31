/* 
 * SYSC 2006 Winter 2016 Labs 7 and 8
 */

#include <stdbool.h>

typedef struct {
    int     *elems;
    int     capacity;   /* Maximum number of elements in the list. */
    int     size; 		/* Current number of elements in the list */
} intlist_t;

/* Functions from Lab 7. */

intlist_t intlist_construct(int capacity);
intlist_t intlist_destroy(intlist_t list);

int intlist_size(const intlist_t list);
int intlist_capacity(const intlist_t list);

intlist_t intlist_append(intlist_t list, int element);
int intlist_get(const intlist_t list, int index);
int intlist_set(intlist_t list, int index, int element);
intlist_t intlist_removeall(intlist_t list);

void    intlist_print(const intlist_t list);

/* Additional functions from Lab 8. */

int intlist_index(const intlist_t list, int target);
int intlist_count(const intlist_t list, int target);
_Bool intlist_contains(const intlist_t list, int target);
intlist_t intlist_delete(intlist_t list, int index);

intlist_t increase_capacity(intlist_t list, int new_capacity);

