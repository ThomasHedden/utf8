#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
is2butf8(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int is a two-byte UTF-8 character;
                otherwise, the return value is false.
Functions used: std library & isb1of2b() & istbutf8()
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        fputu.c, isspaceu.c, putuchar.c, toloweru.c,
                toupperu.c                                */

bool isb1of2b(unsigned int);
bool istbutf8(unsigned int);

bool is2butf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   // byte 1 is the leftmost (highest-order) byte
   unsigned int byte1 = (u & 0xFF000000) >> 24; // byte 1 of passed unsigned int 
   unsigned int byte2 = (u & 0x00FF0000) >> 16; // byte 2 of passed unsigned int
   unsigned int byte3 = (u & 0x0000FF00) >>  8; // byte 3 of passed unsigned int
   unsigned int byte4 =  u & 0x000000FF;        // byte 4 of passed unsigned int

   // all bytes must fall within correct ranges
   if((byte1 == 0x00) && // byte 1 must be zero
      (byte2 == 0x00) && // byte 2 must be zero
      isb1of2b(byte3) && // byte 3 must be 1st byte of 2-byte UTF-8 character
      istbutf8(byte4)) { // byte 4 must be a trailing byte
      return true;
   } else {
      return false;
   }
}; /* end is2butf8() */

