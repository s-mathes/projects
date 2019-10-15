#include <stdio.h>
#include "files.h"
//searchs the given array of struct nodes for a given number
// It returns 1 if the node is found, 0 if it is not found

int search(struct node*LL, int number){

	int current,previous; //index variable
	current=LL[0].next; //move it along to the node the sentinel is pointing at

	if ( LL[0].next == MYNULL){ //sentinel points to nothing, list is empty
		return(0); //nothing is in the list
	}

	while ( LL[current].next != MYNULL ){
		if ( LL[current].data == number ){ //if number is already in the list
			return(1); //return 1 if number is in the list
		}
		else { // if number isn't equal to current data field
			current=LL[current].next; //move along to next node
		}
	}
	if ( LL[current].data == number){ //still need to check the last number
		return(1); //return 1 if number is in the list
	}
	return(0); //if not in the list, return 0
}
