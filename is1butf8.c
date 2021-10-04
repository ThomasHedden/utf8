#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
is1butf8(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int is a single-byte UTF-8
                character; otherwise, the return value
                is false.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        main()                                         */
bool is1butf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   if( u >= 0x00000000 && u <= 0x0000007F ) {
      return true;
   } else {
      return false;
   }
}; /* end is1butf8() */

