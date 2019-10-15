// Kleene Star Operation
// This function accepts two arguments, an NFA and the number of existing states from the main
// function. The Kleene star adds one new state, and three transitions: an epsilon transition
// from the new start state to the old start state, an epsilon transition from the new start state 
// to the old final state, and an epsilon transition from the old final state to the old start
// state. It makes a new NFA whose start state is the new state, and whose accept state is the old
// accept state.
//
//
#include "mylib.h"

NFA Kleene(NFA myNFA, int stateIndex){

	NFA kleeneNFA;
	int start;

	transition* temp;
	transition* test;

	start=stateIndex; // Add a new state
	kleeneNFA.startState=start; // The new state is the new NFA's start state
	kleeneNFA.finalState=start; // New NFA's accept state is also the new state.

        transition *epsilonTrans1=malloc(sizeof(transition));
        transition *epsilonTrans2=malloc(sizeof(transition));
        makeTransition(epsilonTrans1, 'E', kleeneNFA.startState, myNFA.startState); // transition new start to old start
        makeTransition(epsilonTrans2, 'E', myNFA.finalState, kleeneNFA.startState); // transition old final to new start

	// start adding them all to the new kleeneNFA
	kleeneNFA.nextTransition=epsilonTrans1;

	test=kleeneNFA.nextTransition; // add old NFA transitions to new kleene
        while ( test != NULL ){
                temp=test;
                test=test->nextTransition;
        }
        temp->nextTransition=myNFA.nextTransition;

        test=kleeneNFA.nextTransition; // add epsilon transitions to the end of NFA union
        while ( test != NULL ){
                temp=test;
                test=test->nextTransition;
        }
        temp->nextTransition=epsilonTrans2;

	return(kleeneNFA);

}
