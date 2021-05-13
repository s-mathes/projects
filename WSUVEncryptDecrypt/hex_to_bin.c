#include "mylib.h"

// Converts a single hex character to binary.
void hex_to_bin(char ch,char* hex){
     
	if ( ch=='0' ) hex=strcpy(hex,"0000");
	else if (ch=='1') strcpy(hex,"0001");
	else if (ch=='2')strcpy(hex,"0010");
	else if (ch=='3') strcpy(hex,"0011");
	else if (ch=='4') strcpy(hex,"0100");
	else if (ch=='5') strcpy(hex,"0101");
	else if (ch=='6') strcpy(hex,"0110");
	else if (ch=='7') strcpy(hex,"0111");
	else if (ch=='8') strcpy(hex,"1000");
	else if (ch=='9') strcpy(hex,"1001");
	else if (ch=='a')strcpy(hex,"1010");
	else if (ch=='b') strcpy(hex,"1011");
	else if (ch=='c') strcpy(hex,"1100");
	else if (ch=='d')strcpy(hex,"1101");
	else if (ch=='e')strcpy(hex,"1110");
	else if (ch=='f')strcpy(hex,"1111");
	
	/*switch ( ch ) {
	case '0':
            hex=strcpy(hex,"0000");
        case '1':
            strcpy(hex,"0001");
        case '2':
            strcpy(hex,"0010");
        case '3':
            strcpy(hex,"0011");
        case '4':
            strcpy(hex,"0100");
        case '5':
            strcpy(hex,"0101");
        case '6':
            strcpy(hex,"0110");
        case '7':
            strcpy(hex,"0111");
        case '8':
            strcpy(hex,"1000");
        case '9':
            strcpy(hex,"1001");
        case 'a':
            strcpy(hex,"1010");
        case 'b':
            strcpy(hex,"1011");
        case 'c':
            strcpy(hex,"1100");
        case 'd':
            strcpy(hex,"1101");
        case 'e':
            strcpy(hex,"1110");
        case 'f':
            strcpy(hex,"1111");
     }*/
}
