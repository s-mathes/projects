#include "mylib.h"
//Function takes a pointer to a linked list and an NFA. Creates
// a new node with the number in the data field, and inserts the
//new node after the linked lists' sentinel.

int push(node* stack, NFA num){

	node *temp;
	temp=malloc(sizeof(node));

	temp->data=num; // set up a new node with new data
	temp->next=stack->next;

	stack->next=temp;	

}
