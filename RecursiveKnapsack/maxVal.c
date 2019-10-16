#include "mylib.h"
//size is the number of cases, 0-n, in the inventory

int maxVal(int capacity,int size){

	int i=0;
	int highestVal=0;
	int value=0;

	if ( capacity < 0 ){
		return(0);
	}

	while ( i <= size ){
		if ( (capacity-inventory[i].weight) >= 0 ){
			value=inventory[i].value+maxVal(capacity-inventory[i].weight,size);
			if ( value > highestVal ){
				highestVal=value;
			}
		}
		i++;
	}
	return(highestVal);

}
