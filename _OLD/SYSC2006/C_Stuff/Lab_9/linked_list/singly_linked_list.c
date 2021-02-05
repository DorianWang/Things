/* SYSC 2006 Winter 2016 Lab 9
   A module that implements a singly-linked list of integers.
 */

#include <assert.h>  // assert
#include <stdlib.h>  // malloc, free
#include <stdbool.h>
#include <stdio.h>   // printf

#include "singly_linked_list.h"

/*************************************************************
 * Functions presented in lectures.
 *************************************************************/

/* Return a pointer to a new IntNode (a node in a singly-linked list).

   Parameter value: the integer to be stored in the node.
   Parameter next: a pointer to the node that the new node should point to.
   This pointer should be NULL if the new node is the last node in the
   linked list.
   Terminate (via assert) if memory for the node cannot be allocated.
 */
IntNode *intnode_construct(int value, IntNode *next)
{
    IntNode *p = malloc(sizeof(IntNode));
    assert (p != NULL);
    p->value = value;
    p->next = next;
    return p;
}

/* Print the linked list pointed to by p.
 */
void print_linked_list(IntNode *p)
{
	if (p == NULL) {
		printf("empty list");
		return;
 	}

	/* Print every node in the linked list except the last one,
	   using the format: value1 -> value2 -> value3 -> ...
	 */
	for (; p->next != NULL; p = p->next) {
		printf("%d -> ", p->value);
	}

	/* Print the last node. */
	printf("%d", p->value);
}

/* Return true if the linked list pointed to by head contains the
   specified value; otherwise return false.
 */
_Bool contains(IntNode *head, int value)
{
    IntNode *p;

    for (p = head; p != NULL; p = p->next) {
        if (value == p->value) {
            return true;
        }
    }
    return false;
}

/* A version of contains that eliminates local variable p.
 */
_Bool contains_shorter(IntNode *head, int value)
{
    for ( ; head != NULL; head = head->next) {
        if (value == head->value) {
            return true;
        }
    }
    return false;
}

/* Insert a node containing the specified integer value at the front
   of the linked list pointed to by head, and return a pointer to the
   first node in the modified list.
 */
IntNode *insert_front(IntNode *head, int value)
{
    return intnode_construct(value, head);
}

/* Append a node containing the specified integer value at the rear
   of the linked list pointed to by head, and return a pointer to the
   first node in the modified list.

   Exercise - can we eliminate local variable p and instead "walk"
   parameter head down the linked list, as we did with contains_shorter()?
   If not, why not? (Hint: draw a memory diagram showing the activation
   frames for this function and its caller.)
 */
IntNode *append_rear(IntNode *head, int value)
{
    if (head == NULL) {
        /* Appending a node to the end of an empty linked list
         * is the same as inserting a node at the front of an
         * empty linked list.
         */
        head = intnode_construct(value, head /* NULL */);
    } else {
        IntNode *p;

		// Traverse the linked list until p points to the last node.

        for (p = head; p->next != NULL; p = p->next)
        {}

        p->next = intnode_construct(value, p->next /* NULL */);
    }
	return head;
}

/* Remove the node at the front of the linked list pointed to by head,
   and return a pointer to the first node in the modified list.
   Terminate (via assert) if if the linked list is empty.
 */
IntNode *remove_first(IntNode *head)
{
    assert(head != NULL);

	/* We need to retrieve the pointer to the second node (or NULL, if the
	   list contains one node) BEFORE we free the first node.
	   Remember, the result of accessing a node after it has been
	   freed is undefined.
	 */
    IntNode *p = head;
    head = head->next;
    free(p);
	return head;
}

/* Remove the node at the rear of the linked list pointed to by head,
   and return a pointer to the first node in the modified list.
   Terminate (via assert) if the linked list is empty.
 */
IntNode *remove_last(IntNode *head)
{
    assert(head != NULL);

    if (head->next == NULL) {
        /* There is exactly one node in the linked list. */
        free(head);
        head = NULL;
    } else {
        IntNode *p1, *p2 = NULL;
        for (p1 = head; p1->next != NULL; p1 = p1->next) {
            p2 = p1;
        }

        /* Now p1 points to the last node and p2 points to the node before
         * the last node.
         */
        free(p1);
        p2->next = NULL;
    }
	return head;
}

/*****************************************************************
 * Solutions to Lab 9
 *****************************************************************/

// Exercise 1

/* Parameter head points to the first node in a linked list, or is
 * null if the list is empty.
 * Count the number of nodes in the linked list that contain an integer
 * equal to target, and return that count.
 * Return 0 if the linked list is empty.
 */
int count(IntNode *head, int target)
{
   int counter = 0;
    for ( ; head != NULL; head = head -> next) {
        if (target == head -> value) {
            counter++;
        }
    }
    return counter;
}

// Exercise 2

/* Parameter head points to the first node in a linked list, or is
 * null if the list is empty.
 * Return the index (position) of the first node that contains an
 * integer equal to target. The first node is at index 0, the second node
 * is at index 1, etc.
 * Return -1 if target is not in the linked list, or if the linked list
 * is empty.
 */
int index(IntNode *head, int target)
{
   int indexCounter = 0;
   for ( ; head != NULL; head = head -> next) {
      if (target == head -> value) {
         return indexCounter;
      }
      indexCounter++;
   }
   return -1;
}

// Exercise 3

/* Parameter head points to the first node in a linked list, or is
 * null if the list is empty.
 * Return the integer stored in the node at the specified index.
 * The first node is at index 0, the second node is at index 1, etc.
 * The function terminates (via assert) if the linked list is empty or
 * if parameter index is invalid (negative or greater than the number
 * of nodes in the linked list).
 */
int fetch(IntNode *head, int index)
{
   int i = 0;
   for (i = 0; i < index; i ++){
      assert(head != NULL);
      head = head -> next;
   }
   return head -> value;
}

// Exercise 4

IntNode *insert(IntNode *head, int index, int x)
{
   assert(index >= 0);
   int i = 0;
   if (index == 0){
      IntNode* newNode = intnode_construct(x, head);
      return newNode;
   }
   IntNode* newHead = head;
   for (i = 0; i < index - 1; i ++){
      assert(head != NULL);
      head = head -> next;
   }

	IntNode* newNode = intnode_construct(x, head -> next);
	head -> next = newNode;

	return newHead;
}








