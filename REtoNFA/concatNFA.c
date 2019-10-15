// Concatenation Function
// This function accepts two NFAs, and makes a new NFA that 
// appends the second argument to the end of
// the first argument. Returns the new NFA.

#include "mylib.h"

NFA concatNFA(NFA NFA1, NFA NFA2){
	
	transition *temp;
	transition *test;
	NFA concatenatedNFA;

	transition *epsilonTrans;
	epsilonTrans=malloc(sizeof(transition));
	makeTransition(epsilonTrans, 'E', NFA1.finalState, NFA2.startState);

	// Make a new NFA whose start state is NFA1's start state, and accept state is
	// NFA2's accept state.
	concatenatedNFA=makeNFA(NFA1.startState, NFA2.finalState);

	// Add all transitions to the new concatenatedNFA
	concatenatedNFA.nextTransition=NFA1.nextTransition; // add NFA1's transitions to new NFA

	test=concatenatedNFA.nextTransition; // add epsilon transitions
        while ( test != NULL ){
                temp=test;
                test=test->nextTransition;
        }
        temp->nextTransition=epsilonTrans;

	test=concatenatedNFA.nextTransition; // add NFA2 transitions
        while ( test != NULL ){
                temp=test;
                test=test->nextTransition;
        }
        temp->nextTransition=NFA2.nextTransition;


	return(concatenatedNFA);

}
