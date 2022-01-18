#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
bool is3b_Ucp(unsigned int)
Pre:            This function takes an unsigned int representing
                a Unicode code point.
Post:           This function returns boolean true if this
                unsigned int is a Unicode code point that
		should be encoded as a 3-byte UTF-8
                character; otherwise, the return value
                is false.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:                                                       */
bool is3b_Ucp(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // Unicode code point to be encoded with 3 bytes
   if( (u >= 0x00000800) && (u <= 0x0000FFFF) ) {
      return true;
   } else {
      return false;
   }
}; /* end is3b_Ucp() */
