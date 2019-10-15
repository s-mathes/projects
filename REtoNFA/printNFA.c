// Program accepts a single NFA as an argument. Prints the
// NFA and its linked list of transitions in a nicely formatted 
// way.

#include "mylib.h"

void printNFA(NFA myNFA){

	transition *temp; // points to the next transition list

	printf("NFA: \n");
	printf("Start state: q%d \n", myNFA.startState); // print initial state
 
	printf("Accept state: q%d \n", myNFA.finalState);	// print accept state

	temp=myNFA.nextTransition; // point the temp at the transition list and start moving through it

	printf("Transitions: ");
	while ( temp != NULL ){ // print out all the transitions
		if ( temp->nextTransition == NULL ){  // last element, no comma
		                printf("(q%d, %c) -> q%d ", temp->stateOne, temp->symbol, temp->stateTwo);
				temp=temp->nextTransition;
		}
		else {
			printf("(q%d, %c) -> q%d, ", temp->stateOne, temp->symbol, temp->stateTwo);
			temp=temp->nextTransition;
		}
	}
	printf("\n");

	return;

}



