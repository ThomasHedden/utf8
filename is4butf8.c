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
                unsigned int represents a four-byte UTF-8
                character;
                otherwise, the return value is false.
Functions used: standard library & isb1of4b() & istbutf8()
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        fputu.c, isspaceu.c, putuchar.c, toloweru.c,
                toupperu.c                                */

// bool isb1of4b(unsigned int);
bool istbutf8(unsigned int);

bool is4butf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   // byte 1 is the leftmost (highest-order) byte
   // here the first byte of the 4-byte sequence is byte 1
   unsigned int byte1 = (u & 0xFF000000) >> 24; // byte 1 of passed unsigned int 
   unsigned int byte2 = (u & 0x00FF0000) >> 16; // byte 2 of passed unsigned int
   unsigned int byte3 = (u & 0x0000FF00) >>  8; // byte 3 of passed unsigned int
   unsigned int byte4 = (u & 0x000000FF);       // byte 4 of passed unsigned int

   /* Trailing bytes check has to account for the following exceptions:
   if leading byte is 0xF0, range for 1st trailing byte is NOT ...
      if( byte2 >= 0x00000080 && byte2 <= 0x000000BF ) {
   ..., but rather ...
      if( byte2 >= 0x00000090 && byte2 <= 0x000000BF ) {
   if leading byte is 0xF4, range for 1st trailing byte is NOT ...
      if( byte2 >= 0x00000080 && byte2 <= 0x000000BF ) {
   ..., but rather ...
      if( byte2 >= 0x00000080 && byte2 <= 0x0000008F ) {
   See Table 3-7. Well-Formed UTF-8 Byte Sequences
   in Unicode standard
   So, isb1of4b(byte1) cannot be used to check the leading byte,
   and istbutf8(byte2) cannot always be used to check the first
   trailing byte (byte2), because valid range for 1st trailing byte
   (byte2) depends on the value of the leading byte (byte1). */

   // check whether all bytes fall in appropriate ranges
   if( ( (byte1 == 0xF0 )                    &&
        ((byte2 >= 0x90) && (byte2 <= 0xBF)) &&
        istbutf8(byte3)                      &&
        istbutf8(byte4))                     ||
       (((byte1 >= 0xF1) && (byte1 <= 0xF3)) && 
        istbutf8(byte2)                      &&
        istbutf8(byte3)                      &&
        istbutf8(byte4))                     ||
       ( (byte1 == 0xF4 )                    &&
        ((byte2 >= 0x80) && (byte2 <= 0x8F)) &&
        istbutf8(byte3)                      &&
        istbutf8(byte4)) ) {
      return true;
   } else {
      // not all bytes fall in appropriate ranges
      return false;
   }
}; /* end is4butf8() */

