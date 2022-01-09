#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
istbutf8(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int is the trailing byte of a multi-
                byte UTF-8 character; otherwise, the return
                value is false.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        fgetu.c, getu.c, is2butf8.c, is3butf8.c, is4butf8.c*/
bool istbutf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   // check whether u is the trailing byte of a multi-byte character
   if( u >= 0x00000080 && u <= 0x000000BF ) {
      return true;
   } else {
      return false;
   }
}; /* end istbutf8() */


