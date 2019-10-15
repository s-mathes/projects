#include <stdio.h>
#include "files.h"
//print the elements of a linked list in numerical order of the data field
//accepts one argument, the pointer to an array of struct nodes

void print(struct node*LL){
	int i=LL[0].next; //index variable, start at what the sentinel node is pointing at
	
	if ( LL[0].next == MYNULL ){ //list is empty, print nothing
		return;
	}

	while ( LL[i].next != MYNULL ){
		printf("%d ",LL[i].data);
		i=LL[i].next; //move along to next node
	}
        printf("%d\n",LL[i].data); //print last node and a new line
}
