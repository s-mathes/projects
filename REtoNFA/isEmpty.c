#include "mylib.h" // returns a 1 if the stack is empty

int isEmpty(node* stack){

	if ( stack->next == NULL ){
		return 1;
	}
	else {
		return 0;
	}
		
}
