// Sarah Mathes
// CS317 - Automata and Formal Languages, WSU-V Fall 2019
// Project 1 - RE to NFA Converter
// A C program that implements a regular expression to non-deterministic
// finite automata converter. Program reads a string from a text file that
// is a RE arranged in post-fix notation. It then computes
// the equivalent NFA, and prints out the start state, accept state,
// and all transitions.
// When a letter is read, an NFA is created and pushed to the stack.
// When an operator is read, the relevant NFAs are popped and made into
// a new NFA, which is pushed into the stack. If the line ends and there are 
// extra NFAs in the stack, or if it ever tries to pop from an empty stack, 
// then the string was improperly formatted, and an error message is displayed.

#include "mylib.h"

int main(void){
	FILE * fp;
	char filename[200];
	char expression[500];
	node *stack; // Initialize a stack that stores the NFAs.
	stack=malloc(sizeof(node)); 
	stack->next=NULL;
	stack->data.startState=-1; // INITIALIZE BOTTOM OF THE STACK, -1 is a flag that the stack is empty.
	NFA NFAtemp, NFAtemp2, finalNFA;
	int i=0; // index for the expression string
	int j=1; // index for the states
	int accept;
	int start;
	char symbol;
	int flag=0; // flag to indicate an invalid RE

	i=0;
	printf("Please enter the file name: \n");
	scanf("%s",filename);
	printf("Reading expressions from file: %s\n", filename);
	fp=fopen(filename,"r");
	if (fp==NULL){
		printf("Error opening file\n");
		return(0);
	}
	while ( fgets(expression, 500, fp) ) { // reading each line into expression[] one at a time
		printf("RE Expression: %s", expression); // ------------------------------------------NFA LOGIC ALGORITHM
		i=0;
		j=1; // reset the indexes
		flag=0;
			while ( expression[i] != '\n' ) { // while reading the line, push to the stack
				if ( expression[i] == '|' ){ // OR operator, pop stacki
					if ( isEmpty(stack) == 1 ) { // if the stack is empty, there was an invalid RE
						flag=1;
					}
					else NFAtemp=pop(stack); // pop from stack and make NFA1
					if ( isEmpty(stack) == 1 ) { // if the stack is empty, there was an invalid RE
                                                flag=1;
                                        }
					else {
						NFAtemp2=pop(stack); // pop from stack and make NFA2
						finalNFA=makeUnion(NFAtemp,NFAtemp2, j); // OR NFA1 and NFA2 into a new NFA and push to stack	
						j=j+2; // incremement the stack to keep up with the additional stacks from makeUnion() 
						push(stack, finalNFA); // last step, push it into the stack!
					}
				}
				else if ( expression[i] == '*' ){ // Kleene closure, zero or more times
					if ( isEmpty(stack) == 1 ) { // if the stack is empty, there was an invalid RE
                                                flag=1;
                                        }
                                        else {
						NFAtemp=pop(stack); // pop from stack and make NFA
						finalNFA=Kleene(NFAtemp, j); // make NFA that accepts a single expression[i] and push to stack
						j++; // increment the state counter to keep up with the new state from the Kleene star
						push(stack, finalNFA);
					}
				}
				else if ( expression[i] == '&' ) { // Concatenation
					if ( isEmpty(stack) == 1 ) { // if the stack is empty, there was an invalid RE
                                                flag=1;
                                        }
                                        else NFAtemp=pop(stack); // pop from stack and make NFA1
					if ( isEmpty(stack) == 1 ) { // if the stack is empty, there was an invalid RE
                                                flag=1;
                                        }
                                        else {
						NFAtemp2=pop(stack);// pop from stack and make NFA2
						finalNFA=concatNFA(NFAtemp2, NFAtemp); // make NFA that concatenates them 
						push(stack,finalNFA); // push it to stack
					}
				}
				else { // character is a letter, not an expression
					if ( expression[i] == 'a' ){ // make an NFA that accepts a and push it to the stack
						NFAtemp; // make an NFA and push it onto the stack
						start=j; // first state
						j++;
						accept=j;
						j++;
						NFAtemp=makeNFA(start, accept);
						makeTransition(NFAtemp.nextTransition, 'a', start, accept);
						push(stack,NFAtemp); // push it into the stack
					}
					else if ( expression[i] == 'b' ) {
                                                NFAtemp; // make an NFA 
                                                start=j; // first state
                                                j++;
                                                accept=j;
                                                j++;
                                                NFAtemp=makeNFA(start, accept);
                                                makeTransition(NFAtemp.nextTransition, 'b', start, accept);
						push(stack,NFAtemp); // push it into the stack
					}
					else if ( expression[i] == 'c' ){
						NFAtemp; // make an NFA
                                                start=j; // first state
                                                j++;
                                                accept=j;
                                                j++;
                                                NFAtemp=makeNFA(start, accept);
                                                makeTransition(NFAtemp.nextTransition, 'c', start, accept);
                                                push(stack,NFAtemp); // push it into the stack
					}
					else if ( expression[i] == 'd' ){
						NFAtemp; // make an NFA
                                                start=j; // first state
                                                j++;
                                                accept=j;
                                                j++;
                                                NFAtemp=makeNFA(start, accept);
                                                makeTransition(NFAtemp.nextTransition, 'd', start, accept);
                                                push(stack,NFAtemp); // push it into the stack
					}
					else if ( expression[i] == 'e' ){
                                                NFAtemp; // make an NFA
                                                start=j; // first state
                                                j++;
                                                accept=j;
                                                j++;
                                                NFAtemp=makeNFA(start, accept);
                                                makeTransition(NFAtemp.nextTransition, 'd', start, accept);
                                                push(stack,NFAtemp); // push it into the stack
					}
					else if ( expression[i] == 'E' ){ 
                                                NFAtemp; // make an NFA
                                                start=j; // first state
                                                j++;
                                                accept=j;
                                                j++;
                                                NFAtemp=makeNFA(start, accept);
                                                makeTransition(NFAtemp.nextTransition, 'E', start, accept);
                                                push(stack,NFAtemp); // push it into the stack
					}
					else { // invalid expression, print error and leave
						printf("Error: Invalid character. Please enter RE that only contain:\na, b, c, d, e");
						return 0;
					}
				}
				i++; // move along to the next character in the line.
			} // end of while loop, fgets is done with the line. expression[i] is \n, and NFA stack is ready to be checked.
		        if ( expression[0] == '\n' ) { // No input,(empty string) make an NFA with the empty string.
       			        NFAtemp.startState=1;
                		NFAtemp.finalState=2;
		                NFAtemp.nextTransition=malloc(sizeof(transition));
                		NFAtemp.nextTransition->stateOne=1;
		                NFAtemp.nextTransition->stateTwo=2;
                		NFAtemp.nextTransition->symbol='E';
		                NFAtemp.nextTransition->nextTransition=NULL;
               			printNFA(NFAtemp);
		        }
			else if ( flag == 1 ){ // The line was an invalid RE, print an error.
				printf("Invalid Regular Expression.\n");
			}
			else { // Make that NFA!
				finalNFA=pop(stack); // At this point, there should only be one NFA in the stack.
				if ( isEmpty(stack) == 0 ){ // If there wasn't, it's an invalid RE
		                            printf("Invalid Regular Expression.\n");
					    while ( isEmpty(stack) == 0 ){// While there are still NFAs in the stack, empty it out
						  pop(stack);
					    }
                      		}
				else printNFA(finalNFA); // It's a valid RE, print its NFA!
			}
			printf("\n");
		}

	fclose(fp);

	return 0;

}
