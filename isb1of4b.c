#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
isb1of4b(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int is the first byte of a four-byte
                UTF-8 character; otherwise, the return value
                is false.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        main()                                         */
bool isb1of4b(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   if( u >= 0x000000F0 && u <= 0x000000F4 ) {
      return true;
   } else {
      return false;
   }
}; /* end isb1of4b() */




