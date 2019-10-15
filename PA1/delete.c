#include <stdio.h>
#include "files.h"
//function that deletes a number from a linked list (LL)
//function accepts two arguments: the list and the number that will be deleted
//main program handles error cases (number not in list, empty list)

int delete(struct node*LL, int number){

	int current,previous;
	previous=0; //start at sentinel	
	current=LL[previous].next;

	while ( LL[current].data != number ){ //move through LL until you find the number
		current=LL[current].next; //move current along to next node
		previous=LL[previous].next; //update previous to its next node
	}
	//when it leaves the while loop, current should be the index of the node to be deleted.
	LL[previous].next=LL[current].next; //point previous to the node after current
	release_node(LL,current); //delete that index from the linked list
	return(0);
}
