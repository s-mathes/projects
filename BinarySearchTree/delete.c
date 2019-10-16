#include "mylib.h"
//main program already searched tree
//so we know that number is in the tree.

node* delete(node* root,int number){
	node*largest;
	node*parent;
	node*temp;
	
	if ( number < root->data ){ //data is smaller, lies in the left subtree
		root->left=delete(root->left,number);
		return(root);
	}
	else if ( number > root->data ){ //data is larger, lies in the right subtree
		root->right=delete(root->right,number);
		return(root); 
	}
	else { //root->data is number, needs to be deleted
		if( root->left == NULL && root->right == NULL ){ //root has no left/right nodes
			free(root);
			root=NULL;
			return(root); //will free the node and return a null pointer
		}
		else if( root->left == NULL ){ //root only has right node
			temp=root;
			root=root->right;
			free(temp); //clear root
			return(root); //return value of right child. hoists it up into the space of its parent.
		}
		else if( root->right == NULL ){ //root only has left node
			temp=root;
			root=root->left; 
			free(temp); //clear root
			return(root); //return value of left child
		}
		else { //root has right and left nodes
			temp=find_largest(root->left); //point temp at largest node in the left subtree
			root->data=temp->data; //replace node with the largest
			root->left=delete(root->left,temp->data); //goes down and "deletes" the largest node
		}
	}
	return(root);
//algorithm as first discussed in class, couldn't get this to work
//may revisit, but I think I need to address each case for how mant children a node has
//	else if ( number == root->data ){ //if the data is the same as number, it needs to be deleted
//		largest=find_largest(root->left);
//		parent=largest_parent(root->left);
//
//		parent->right=largest->left;
//		largest->left=root->left;
//		largest->right=root->right;
//
//		free(root);
//		return(largest);
//	}
}

