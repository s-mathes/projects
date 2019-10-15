#include "mylib.h"
//Function takes a pointer to a linked list. Deletes the node
//after the sentinel, and pops the NFA field of the deleted node.

NFA pop(node*stack){

	node *temp;
	NFA number;

	number=stack->next->data;
	temp=stack->next->next; //save second node
	free(stack->next);
	stack->next=temp;

	return(number);
}
