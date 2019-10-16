#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int first_print; //global variable to format commas in print functions

struct node { 
	int data; 
	struct node *left;
	struct node *right; 
}; 

typedef struct node
	node;

void inorder(node*); //print tree in LNR order
node* add(node*,int); //add a number to the tree
node* search(node*,int); //search for a number in the tree, return NULL if not found
node* delete(node*,int); //delete a number in the tree
node* find_largest(node*); //find the largest number in the tree
node* largest_parent(node*); //find parent of largest number in tree
void postorder(node*); //print tree in LRN order
void preorder(node*); //print tree NLR order
int height(node*); //measure the height of the tree
void print_level(node*,int); //print a given level of the tree
void free_level(node*,int); //free a given level of the tree
