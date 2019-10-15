Sarah Mathes

CS 317, Program 1: Regular Expression to NFA Converter
This program reads a regular expression, or a series of regular expressions
separated by new lines, and converts them to a non deterministic finite
automata, which is described through a list of transitions. The regular
expressions must be in postfix notation.

TO BUILD: Unzip files and compile them with the 'make' command. Execute the 'main'
file. The program will prompt you for the name of the .txt file that you want to use
as input. The text file of example REs is included in the zip file. To use that as
input, type 'examples.txt' when prompted.

CONTENTS:
Makefile - A Makefile to assemble all .c, .h, and .o files into an executable 'main'
concatNFA.c - a function to concatenate one NFA after another
examples.txt - postfix REs from project assignment, can be used as input
isEmpty.c - a function that check if a stack of NFAs is empty
Kleene - function that performs the Kleene star operation on an NFA
main.c - main function, reads input and processes it, and pushes/pop the stack accordingly
makeNFA.c - given a start state, accept state, and transition pointer, this function creates and NFA
makeTransition.c - Creates a transition structure
makeUnion.c - performs the union operator on two NFAs
mylib.h - contains all structure declarations and function prototypes
pop.c - given a stack, pops and returns an NFA
printNFA.c - given an NFA, formats and prints the start/accept states and transitions
push.c - given a stack and an NFA, this function pushes the NFA to the stack