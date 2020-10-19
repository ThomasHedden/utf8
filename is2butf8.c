#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
is2butf8(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int is a two-byte UTF-8 character;
                otherwise, the return value is false.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        main()                                         */
bool is2butf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(44);
   }

   // byte 1 is the leftmost byte
   unsigned int byte1 = (u & 0xFF000000) >> 24; // byte 1 of passed unsigned int 
   unsigned int byte2 = (u & 0x00FF0000) >> 16; // byte 2 of passed unsigned int
   unsigned int byte3 = (u & 0x0000FF00) >>  8; // byte 3 of passed unsigned int
   unsigned int byte4 =  u & 0x000000FF;        // byte 4 of passed unsigned int

   // all bytes fall within correct ranges
   if((byte1 == 0) &&                          // byte 1 must be zero
      (byte2 == 0) &&                          // byte 2 must be zero
      (byte3 >= 0xC2) && (byte3 <= 0xDF) &&    // range for byte 1 of 2-byte
      (byte4 >= 0x80) && (byte4 <= 0xBF)) {    // range for trailing byte
      fprintf(stderr, "%c%c%c%c is a two-byte UTF-8 character\n",
                       (unsigned char) byte1,
                       (unsigned char) byte2,
                       (unsigned char) byte3,
                       (unsigned char) byte4);
      return true;
   } else {
      fprintf(stderr, "0x%X is not a two-byte UTF-8 character\n", u);

      if(byte1 != 0) {
         fprintf(stderr, "   0x%X byte 1 is not 0\n", byte1);
      }
      if(byte2 != 0) {
         fprintf(stderr, "   0x%X byte 2 is not 0\n", byte2);
      }
      if(byte3 < 0xC2) {
         fprintf(stderr, "   0x%X byte 3 is not >= 0xC2\n", byte3);
      }
      if(byte3 > 0xDF) {
         fprintf(stderr, "   0x%X byte 3 is not <= 0xDF\n", byte3);
      }
      if(byte4 < 0x80) {
         fprintf(stderr, "   0x%X byte 4 is not >= 0x80\n", byte4);
      }
      if(byte4 > 0xBF) {
         fprintf(stderr, "   0x%X byte 4 is not <= 0xBF\n", byte4);
      }
      return false;
   }
}; /* end is2butf8() */

