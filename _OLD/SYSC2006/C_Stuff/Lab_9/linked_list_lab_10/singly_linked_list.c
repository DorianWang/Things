/* SYSC 2006 Winter 2016 Lab 10
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

/*-------------------------------------------------------------------*/

/* Exercise 1 */

void remove_duplicates(IntNode *head)
{
   assert(head != NULL);
   while (head -> next != NULL) {
      if (head -> value == (head -> next) -> value){
         IntNode* tempNode = head -> next;
         //printf(head -> value);
         head -> next = (head -> next) -> next;
         free(tempNode);
      }
      else{
         head = head -> next;
      }
   }
}


/* Exercise 2 */

IntNode *reverse(IntNode *head)
{
   if (!head) return NULL;
   IntNode* tempNode;
   IntNode* oldNode = NULL;
   while(head -> next != NULL){
      tempNode = head -> next;
      head -> next = oldNode;
      oldNode = head;
      head = tempNode;
   }
   head -> next = oldNode;
   return head;
}







