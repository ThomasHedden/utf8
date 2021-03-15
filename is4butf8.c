#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
*                      NEEDED ENHANCEMENTS                     *
***************************************************************/
/* Need to test each byte for for validity and output appropriate
   error message identifying which byte is not in the correct
   range.
*/

/***************************************************************
is4butf8(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int is a four-byte UTF-8 character;
                otherwise, the return value is false.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        main()                                         */
bool is4butf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   // byte 1 is the leftmost (highest-order) byte
   unsigned int byte1 = (u & 0xFF000000) >> 24; // byte 1 of passed unsigned int 
   unsigned int byte2 = (u & 0x00FF0000) >> 16; // byte 2 of passed unsigned int
   unsigned int byte3 = (u & 0x0000FF00) >>  8; // byte 3 of passed unsigned int
   unsigned int byte4 = (u & 0x000000FF);       // byte 4 of passed unsigned int

   // check whether all bytes fall in appropriate ranges
   if((byte1 >= 0xF0) && (byte3 <= 0xF4) &&     // range for byte 1 of 4-byte
      (byte2 >= 0x80) && (byte4 <= 0xBF) &&     // range for trailing byte
      (byte3 >= 0x80) && (byte4 <= 0xBF) &&     // range for trailing byte
      (byte4 >= 0x80) && (byte4 <= 0xBF)) {     // range for trailing byte
      return true;
   } else {
      // not all bytes fall in appropriate ranges
      return false;
   }
}; /* end is4butf8() */


