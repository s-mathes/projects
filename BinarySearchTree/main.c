//Sarah Robison-Mathes
//CSE 222A, Winter 2019 - Introduction to Data Structures
//Program 4: Binary Search Trees
//This program uses recursion to manipulate a binary search tree.
//Gather the users input to add, delete, or search for a number
//in a BST. When submitting the command "X", program
//should print the BST in preorder, postorder, in order,
//and breadth first order, as well as its height and whether it's
//balanced or not. Program should free all data when it exits.
//See "mylib.h" and each function .c file for more detailed description

#include "mylib.h"

int main(void){

	char input[100]; //stores user input
	node *tree=NULL; //initialize an empty tree
	int number; //stores number from user input
	int i=0; //index for while loops
	int levels; //stores height of tree from height(node*)

	while ( 1 == 1 ){ 
		printf("> ");
		fgets(input,100,stdin); 
		if ( 1 == sscanf(input,"%d", &number) ){ //------------------------------INSERT A NUMBER
			if ( search(tree,number) != NULL ){ 
				printf("%d IS ALREADY IN THE TREE.\n",number);
			}
			else tree=add(tree,number);
		}
		else if ( input[0] == 'X' ){ //------------------------------------------PRINT INFO
			printf("NLR: ");
			first_print=1; //use global variable first-print to format commas 
			preorder(tree); //----------PREORDER PRINT
			printf("\n");
			printf("LNR: ");
			first_print=1; //reset first_print 
			inorder(tree); //----------INORDER PRINT
			printf("\n");
			printf("LRN: ");
                        first_print=1; //reset first_print
			postorder(tree); //--------POSTORDER PRINT
			printf("\n");
			printf("BFS: "); 
			levels=height(tree);
			first_print=1; //reset first_print
			while ( i <= levels ){  //----------BREADTH ORDER
				print_level(tree,i); //print each level, starting at the highest level
				i++;  //increment i from 0 up to "levels"
			}
			printf("\n");
			levels=height(tree)-1;
			printf("HEIGHT: %d\n",levels);
			if ( tree == NULL ){ //if tree is empty, don't state if it's balanced
				printf("TREE IS EMPTY\n");
			}
			else { //not empty, state if balanced or not
				int left_sub=height(tree->left); //height of the left tree
				int right_sub=height(tree->right); //height of the right tree
				if( (left_sub-right_sub) > 1 || (right_sub-left_sub) > 1 ){ //not balanced if difference is more than 1
					printf("BALANCED: NO\n");
				}
				else printf("BALANCED: YES\n");
			}
		}
		else if ( input[0] == 'Q' ){ //---------------------------------QUIT PROGRAM
                        printf("FREEING MEMORY...\n");
			levels=height(tree);
                        while ( levels > 0 ){ //frees all nodes, starting at the lowest level
                                free_level(tree,levels);
                                levels--;
                        }
			return(0);
		}
		else if ( 1 == sscanf(input,"s %d", &number) ){ //--------------SEARCH FOR #
			if ( search(tree,number) == NULL ){ 
				printf("%d is not in tree.\n",number);
			}
			else printf("%d is in tree.\n",number);
		}
		else if ( 1 == sscanf(input,"d %d",&number) ){ //-----------------DELETE #
			if ( search(tree,number) == NULL ){
				printf("%d is not in the tree.\n",number);
			}
			else tree=delete(tree,number);
		}
		else if ( input[0] == '?' ){ //-------------------------------------------DISPLAY INSTRUCTIONS
			printf("#	ENTER A NUMBER INTO THE TREE\nd #	DELETE A NUMBER FROM THE TREE\ns #	SEARCH FOR A NUMBER IN THE TREE\nX	DISPLAY INFORMATION ABOUT THE TREE\nQ	CLEAR MEMORY AND QUIT PROGRAM\n");
		}
		else {
			printf("PLEASE ENTER ? FOR INSTRUCTIONS\n");
		}



	}//end of main loop
}
