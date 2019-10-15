#include "mylib.h"
// Accepts two arguments, a startState and a finalState
// Returns an NFA with the respective fields set and the transition list
// initiated to an address.

NFA makeNFA(int startState, int finalState){
		
	NFA myNFA;

	myNFA.startState=startState;
	myNFA.finalState=finalState;

	myNFA.nextTransition=malloc(sizeof(transition));

	return(myNFA);
}	
