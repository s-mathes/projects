#include "mylib.h"

result maxValue(int capacity,int size){

	int i=0;
	result tempResult;
	result bestResult;
	bestResult.value=0;
	tempResult.value=0;

	if ( computedResults[capacity].value != 0 ){ //if it isn't zero, it's been computed already
		return(computedResults[capacity]);
	}
	
	while ( i <= size){ //initialize all count values to zero
		bestResult.count[i]=0;
		i++;
	}

	if ( capacity < 0 ){
		return(tempResult);
	}

	i=0;
	while ( i <= size){
		if ( (capacity-inventory[i].weight) >= 0 ){
			tempResult=maxValue(capacity-inventory[i].weight,size);
			if ( (inventory[i].value+tempResult.value) > bestResult.value ){
				bestResult=tempResult;
				bestResult.value=bestResult.value+inventory[i].value;
				bestResult.count[i]++;
				computedResults[capacity]=bestResult;
			}
		}
	i++;
	}
	return(bestResult);
}
