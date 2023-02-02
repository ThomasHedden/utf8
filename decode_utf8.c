#include <stdio.h>
#include <stdlib.h>   // needed for exit()
#include <stdbool.h>  // needed for is1butf8() etc.

/***************************************************************
 * long int decode_utf8(unsigned int)
 * This function takes an unsigned int representing a UTF-8
 * character and converts it into the corresponding Unicode
 * code point in the form U+(hex value), but without the U+.
 * For example, 0xD096, which corresponds to Russian capital
 * letter Ж (ZH) is converted into Unicode code point 0416.
 * Written by Thomas Hedden April 2021, modified Jan 2022.
 * Modified Jan 2023:
 * If this function encounters a UTF-8 BOM, it converts it
 * into the bigendian UTF-16 BOM 0xFEFF and promotes it 
 * to the high-order half of its long int return value.
 * The calling function can use the BOM if desired by
 * demoting it >> 32, or it can simply ignore it.
 * Pre:            an unsigned int representing a UTF-8
 *                 character.
 * Post:           an long unsigned int representing the
 *                 Unicode code point corrsponding
 *                 to that UTF-8 character
 * Functions used: standard library functions, is1butf8(), etc.
 * Includes:       stdio.h, stdlib.h, stdbool.h
 * Used in:        utod.c, xtoU.c                          */

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's and #define's should be bracketed by INFDEFINE's
*/

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
bool  is1butf8(unsigned int); // returns true if 1-byte UTF-8
bool  is2butf8(unsigned int); // returns true if 2-byte UTF-8
bool  is3butf8(unsigned int); // returns true if 3-byte UTF-8
bool  is4butf8(unsigned int); // returns true if 4-byte UTF-8

long int decode_utf8(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   // Here byte 1 is the leftmost (highest-order) byte in the int.
   unsigned int byte1 = (u & 0xFF000000) >> 24; // byte 1 of passed unsigned int 
   unsigned int byte2 = (u & 0x00FF0000) >> 16; // byte 2 of passed unsigned int
   unsigned int byte3 = (u & 0x0000FF00) >>  8; // byte 3 of passed unsigned int
   unsigned int byte4 =  u & 0x000000FF;        // byte 4 of passed unsigned int
   // NOTE: Korpela calls the highest-order byte in all UTF-8 characters octet 1.
   // So, he refers to a 1-byte UTF-8 character as "octet 1"; I call it "byte 4".
   // In a 2-byte UTF-8 character, Korpela refers to these two bytes as octets
   // 1 and 2. I would call them bytes 3 and 4.

   // holds return value of this function
   unsigned long int ucp = 0x0000000000000000;

   if(is1butf8(u)) {
      /* from Korpela p. 298
				Hex value of UTF-8 character
         Unicode code point	Octet 1		Octet 2		Octet 3		Octet 4
         00000000 0xxxxxxx	0xxxxxxx */
      ucp = byte4;
      return(ucp);
   }
   if(is2butf8(u)) {
      /* from Korpela p. 298
				Hex value of UTF-8 character
         Unicode code point	Octet 1		Octet 2		Octet 3		Octet 4
         00000yyy yyxxxxxx	110yyyyy	10xxxxxx */
      // byte 1 of ucp is ((byte3 & 00011100) >> 2)
      // byte 2 of ucp is ((byte3 & 00000011) << 6) PLUS (byte4 & 00111111)
      // ucp = (((byte3 & 00011100) >> 2) << 8) + (((byte3 & 00000011) << 6) + (byte4 & 00111111));
      ucp =    (((byte3 & 0x1C)     >> 2) << 8) + (((byte3 & 0x03)     << 6) + (byte4 & 0x3F));
      return(ucp);
   }
   if(is3butf8(u)) {
      /* from Korpela p. 298
				Hex value of UTF-8 character
         Unicode code point	Octet 1		Octet 2		Octet 3		Octet 4
         zzzzyyyy yyxxxxxx	1110zzzz	10yyyyyy	10xxxxxx */
      // byte 1 of ucp is ((byte2 & 00001111) << 4) PLUS ((byte3 & 00111100) >> 2)
      // byte 2 of ucp is ((byte3 & 00000011) << 6) PLUS  (byte4 & 00111111)
      // ucp = ( ( (((byte2 & 00001111) << 4)  +  ((byte3 & 00111100) >> 2)) << 8 ) + 
      //           (((byte3 & 00000011) << 6)  +   (byte4 & 00111111)) );
      ucp = ( ( (((byte2 & 0x0F) << 4)  +  ((byte3 & 0x3C) >> 2)) << 8 ) + 
                (((byte3 & 0x03) << 6)  +   (byte4 & 0x3F)) );
      if(ucp == 0xFEFF) { // big-endian UTF-16 BOM
         ucp = (ucp << 32); // promote to high order part of unsigned long int
      }
      return(ucp);
   }
   if(is4butf8(u)) {
      /* from Korpela p. 298
				Hex value of UTF-8 character
         Unicode code point		Octet 1		Octet 2		Octet 3		Octet 4
         000uuuww zzzzyyyy yyxxxxxx	11110uuu	l0wwzzzz	l0yyyyyy	10xxxxxx */
      // byte 1 of ucp is ((byte1 & 00000111) << 3) PLUS (((byte2) & 00110000) >> 4)
      // byte 2 of ucp is ((byte2 & 00001111) << 4) PLUS (((byte3) & 00111100) >> 2)
      // byte 3 of ucp is ((byte3 & 00000011) << 6) PLUS ((byte4) & 00111111)
      // ucp = (((((byte1 & 00000111) << 3) + ((byte2 & 00110000) >> 4) ) << 16) +
      //        ((((byte2 & 00001111) << 4) + ((byte3 & 00111100) >> 2) ) <<  8) +
      //          ((byte3 & 00000011) << 6) +  (byte4 & 00111111)     );
      ucp = (((((byte1 & 0x07) << 3) + ((byte2 & 0x30) >> 4) ) << 16) +
             ((((byte2 & 0x0F) << 4) + ((byte3 & 0x3C) >> 2) ) <<  8) +
               ((byte3 & 0x03) << 6) +  (byte4 & 0x3F)     );
      return(ucp);
   }
   fprintf(stderr, "0x%X is not a UTF-8 character\n", u);
   return(0xFFFD);
}

