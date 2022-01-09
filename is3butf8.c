#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
*                      NEEDED ENHANCEMENTS                     *
***************************************************************/

/***************************************************************
is3butf8(unsigned int)
Pre:            This function takes an unsigned int.
Post:           This function returns boolean true if this
                unsigned int represents a three-byte UTF-8
                character; otherwise, it returns false.
Functions used: standard library and isb1of3b() istbutf8()
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        fputu.c, isspaceu.c, putuchar.c, toloweru.c,
                toupperu.c                                */

bool istbutf8(unsigned int);

bool is3butf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   // byte 1 is the (highest-order) leftmost byte, so
   // here the first byte of the 3-byte sequence is byte 2
   unsigned int byte1 = (u & 0xFF000000) >> 24; // byte 1 of passed unsigned int 
   unsigned int byte2 = (u & 0x00FF0000) >> 16; // byte 2 of passed unsigned int
   unsigned int byte3 = (u & 0x0000FF00) >>  8; // byte 3 of passed unsigned int
   unsigned int byte4 =  u & 0x000000FF;        // byte 4 of passed unsigned int

   /* Trailing bytes check has to account for the following exceptions:
   if leading byte is 0xE0, range for 1st trailing byte is NOT ...
      if( byte3 >= 0x00000080 && byte3 <= 0x000000BF ) {
   ..., but rather ...
      if( byte3 >= 0x000000A0 && byte3 <= 0x000000BF ) {
   if leading byte is 0xED, range for 1st trailing byte is NOT ...
      if( byte3 >= 0x00000080 && byte3 <= 0x000000BF ) {
   ..., but rather ...
      if( byte3 >= 0x00000080 && byte3 <= 0x0000009F ) {
   See Table 3-7. Well-Formed UTF-8 Byte Sequences
   in Unicode standard
   So, isb1of3b(byte2) cannot be used to check the leading byte,
   and istbutf8(byte3) cannot always be used to check the first
   trailing byte (byte3), because valid range for 1st trailing byte
   (byte3) depends on the value of the leading byte (byte2). */

   // check whether all bytes fall in appropriate ranges
   if((byte1 == 0x00)  && // byte 1 must be zero
      ( ((byte2 == 0xE0) && ((byte3 >= 0xA0)  && (byte3 <= 0xBF))) ||
       (((byte2 >= 0xE1) &&  (byte2 <= 0xEC)) && istbutf8(byte3))  ||
        ((byte2 == 0xED) && ((byte3 >= 0x80)  && (byte3 <= 0x9F))) ||
       (((byte2 >= 0xEE) &&  (byte2 <= 0xEF)) && istbutf8(byte3))) &&
      istbutf8(byte4) ) {  // byte4 is in range for trailing byte
      return true;
   } else {
      // not all bytes fall in appropriate ranges
      return false;
   }
}; /* end is3butf8() */

