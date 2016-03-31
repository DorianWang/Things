/* SYSC 2006 Winter 2016 Lab 9 */

/* A node in a singly-linked list that stores values of type int. */

struct intnode {
    int    value;
    struct intnode *next;
};

typedef struct intnode IntNode;

/* Functions presented in lectures. */

IntNode *intnode_construct(int value, IntNode *next);

void print_linked_list(IntNode *p);
_Bool contains(IntNode *head, int value);
_Bool contains_shorter(IntNode *head, int value);
IntNode *insert_front(IntNode *head, int value);
IntNode *append_rear(IntNode *head, int value);
IntNode *remove_first(IntNode *head);
IntNode *remove_last(IntNode *head);

/* Functions defined in Lab 10. */

int count(IntNode *head, int target);
int index(IntNode *head, int target);
int fetch(IntNode *head, int index);
IntNode *insert(IntNode *head, int index, int x);


