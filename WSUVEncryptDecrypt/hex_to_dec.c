#include "mylib.h"

//convert one hex character ( a letter ) to it's decimal equivalent.
// intended to index the Ftable.
int hex_to_dec(char ch){

	if ( ch == '0' ) return(0);
	if ( ch == '1' ) return(1);
	if ( ch == '2' ) return(2);
	if ( ch == '3' ) return(3);
	if ( ch == '4' ) return(4);
	if ( ch == '5' ) return(5);
	if ( ch == '6' ) return(6);
	if ( ch == '7' ) return(7);
	if ( ch == '8' ) return(8);
	if ( ch == '9' ) return(9);
	if ( ch == 'a' ) return(10);
	if ( ch == 'b' ) return(11);
	if ( ch == 'c' ) return(12);
	if ( ch == 'd' ) return(13);
	if ( ch == 'e' ) return(14);
	if ( ch == 'f' ) return(15);
	else return (-1);

}
