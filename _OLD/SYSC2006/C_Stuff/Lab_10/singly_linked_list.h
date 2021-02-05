/* SYSC 2006 Winter 2016 Lab 10 */

/* A node in a singly-linked list that stores values of type int. */

struct intnode {
    int    value;
    struct intnode *next;
};

typedef struct intnode IntNode;

/* Functions presented in lectures. */

IntNode *intnode_construct(int value, IntNode *next);

void print_linked_list(IntNode *p);

/* Functions written in Lab 10. */

void remove_duplicates(IntNode *head);
IntNode *reverse(IntNode *head);
