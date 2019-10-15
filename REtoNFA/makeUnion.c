// Function accepts three arguments, two NFAs and the number of
// existing states, and creates a new NFA that unions them. The new NFA
// makes a new start state, a new accept state, and makes the four appropriate
// epsilon transitions to NFA1 and NFA2's start and accept states.
// Returns the new NFA.

#include "mylib.h"

NFA makeUnion(NFA NFA1, NFA NFA2, int stateIndex){

	NFA NFAunion;
	int start;
	int accept;
	transition* temp;
	transition* test;

	start=stateIndex++;
	NFAunion.startState=start;	// make a new start state
	accept=stateIndex++;
	NFAunion.finalState=accept; // make a new final state

	transition *epsilonTrans1=malloc(sizeof(transition));
	transition *epsilonTrans2=malloc(sizeof(transition));
	makeTransition(epsilonTrans1, 'E', start, NFA1.startState); // epsilon transition new start state to NFA1's start state
	makeTransition(epsilonTrans2, 'E', start, NFA2.startState); // same thing for NFA2.

	// add all transitions from to the new NFA	
	NFAunion.nextTransition=epsilonTrans1; // add new epsilon transitions
	NFAunion.nextTransition->nextTransition=epsilonTrans2;
	
	epsilonTrans2->nextTransition=NFA1.nextTransition;

	test=NFAunion.nextTransition; // add NFA2 transitions to the end of NFA union
	while ( test != NULL ){ 
		temp=test;
		test=test->nextTransition;
	}
	temp->nextTransition=NFA2.nextTransition;

        transition *epsilonEnd1=malloc(sizeof(transition));
        transition *epsilonEnd2=malloc(sizeof(transition));
        makeTransition(epsilonEnd1, 'E', NFA1.finalState, accept); // epsilon transition NFA1 accept state to new accept state
        makeTransition(epsilonEnd2, 'E', NFA2.finalState, accept); // same thing for NFA2

        test=NFAunion.nextTransition; // add epsilon transitions to the end of NFA union
        while ( test != NULL ){
                temp=test;
                test=test->nextTransition;
        }
        temp->nextTransition=epsilonEnd1;
        test=NFAunion.nextTransition; // add epsilon transitions to the end of NFA union
        while ( test != NULL ){
                temp=test;
                test=test->nextTransition;
        }
        temp->nextTransition=epsilonEnd2;

	return(NFAunion);
}
