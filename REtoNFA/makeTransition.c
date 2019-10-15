#include "mylib.h"

int makeTransition(transition *nextTransition, char symbol, int firstState, int secondState){ // add a new transition
	
	nextTransition->stateOne=firstState;
	nextTransition->stateTwo=secondState;	
	nextTransition->symbol=symbol;
	nextTransition->nextTransition=NULL; // point the next transition at nothing

	return 0;
	

}
