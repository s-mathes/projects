#include <stdio.h>
#include <stdlib.h>

struct transition {
        int stateOne;
        int stateTwo;
        char symbol; // a, b, c, d, f
        struct transition* nextTransition; // point it at nothing until changed
};

struct NFA {
        int startState;
        int finalState;
        struct transition* nextTransition;
};

struct node { // Each NFA that gets stored in a stack of the stack
	struct NFA data; 
	struct node *next;
};

typedef struct node
	node;

typedef struct transition
	transition;

typedef struct NFA
	NFA;

NFA concatNFA(NFA NFA1, NFA NFA2);
int push(node* stack, NFA data);
NFA pop(node*);
int isempty(node* list);
int makeTransition(transition *nextTransition, char symbol, int firstState, int secondState);
NFA makeNFA(int startState, int finalState);
void printNFA(NFA);
NFA makeUnion(NFA NFA1, NFA NFA2, int stateIndex);
int isEmpty(struct node* stack);
NFA Kleene(NFA myNFA, int stateIndex);


