/* SYSC 2006 Winter 2016 Lab 10 */

/* Test harness to exercise the functions in singly_linked_list.c.
 */

#include <assert.h>  // assert
#include <stdlib.h>  // malloc, free
#include <stdbool.h>
#include <stdio.h>   // printf

#include "singly_linked_list.h"

/* Free all the nodes in the linked list pointed to by head.
 */
void free_linked_list(IntNode *head)
{
    for (IntNode *p = head; p != NULL; ) {
        head = head->next;
        free(p);
        p = head;
    }
}

int main(void)
{
	IntNode *head;

    /* Tests for Exercise 1. */

    printf("=== Testing remove_duplicates ===\n\n");
    
    printf("Test 1\n");

    printf("Building linked list: ");   
    head = NULL;
    head = intnode_construct(5, head);
    head = intnode_construct(3, head);
    head = intnode_construct(3, head);
    head = intnode_construct(4, head);
    head = intnode_construct(4, head);
    head = intnode_construct(4, head);
    head = intnode_construct(1, head);
    head = intnode_construct(3, head);
    head = intnode_construct(3, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling remove_duplicates\n");
    remove_duplicates(head);
    printf("Expected result: 3 -> 1 -> 4 -> 3 -> 5\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 2\n");
    
    printf("Building linked list: ");
    head = NULL;
    head = intnode_construct(5, head);
    head = intnode_construct(5, head);
    head = intnode_construct(5, head);
    head = intnode_construct(5, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling remove_duplicates\n");
    remove_duplicates(head);
    printf("Expected result: 5\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 3\n");

    printf("Building linked list: ");  
    head = NULL;
    head = intnode_construct(2, head);
    head = intnode_construct(2, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling remove_duplicates\n");
    remove_duplicates(head);
    printf("Expected result: 2\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 4\n");

    printf("Building linked list: ");    
    head = NULL;
    head = intnode_construct(4, head);
    head = intnode_construct(3, head);
    head = intnode_construct(2, head);
    head = intnode_construct(1, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling remove_duplicates\n");
    remove_duplicates(head);
    printf("Expected result: 1 -> 2 -> 3 -> 4\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 5\n");
 
    printf("Building linked list: ");   
    head = NULL;
    head = intnode_construct(4, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling remove_duplicates\n");
    remove_duplicates(head);
    printf("Expected result: 4\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    /* Tests for Exercise 2. */

    printf("=== Testing reverse ===\n\n");

    printf("Test 1\n");

    printf("Building linked list: ");    
    head = NULL;
    head = intnode_construct(4, head);
    head = intnode_construct(3, head);
    head = intnode_construct(2, head);
    head = intnode_construct(1, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling reverse\n");
    head = reverse(head);
    printf("Expected result: 4 -> 3 -> 2 -> 1\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 2\n");

    printf("Building linked list: ");    
    head = NULL;
    head = intnode_construct(2, head);
    head = intnode_construct(1, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling reverse\n");
    head = reverse(head);
    printf("Expected result: 2 -> 1\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 3\n");

    printf("Building linked list: ");    
    head = NULL;
    head = intnode_construct(2, head);

    print_linked_list(head);
    printf("\n");

    printf("Calling reverse\n");
    head = reverse(head);
    printf("Expected result: 2\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);

    printf("Test 4\n");

    printf("Building linked list: ");    
    head = NULL;

    print_linked_list(head);
    printf(" (head == NULL)\n");

    printf("Calling reverse\n");
    head = reverse(head);
    printf("Expected result: empty list\n");
    printf("  Actual result: ");
    print_linked_list(head);
    printf("\n\n");

    free_linked_list(head);
}
