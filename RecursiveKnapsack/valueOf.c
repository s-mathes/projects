#include "mylib.h"

int valueOf(item*inventory,int size){

	int i=0;
	int value=0;

	while ( i <= size ){
		value=value+(inventory[i].value*inventory[i].quantity); //keep a running sum of items and value
		i++;
	}
	
	return(value);

}
