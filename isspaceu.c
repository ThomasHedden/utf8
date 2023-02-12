#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <ctype.h>   // needed for isspace()
#include <stdbool.h> // needed for is1butf8() etc.

/***************************************************************
* bool isspaceu(unsigned int)
* This function takes an unsigned four-byte int representing
* a UTF-8 character, and returns boolean true if this UTF-8
* character represents any kind of space. Otherwise, it
* returns false. For 1-byte UTF-8 characters it uses the
* ctype.h library function isspace(). For 2-byte and 3-byte
* UTF-8 characters it attempts to check against all possible
* space characters and returns true if it finds one. It is
* assumed that no 4-byte UTF-8 characters represent spaces.
* The only space character that does not return true is the
* zero width no-break space U+FEFF, the UTF-8 BOM 0xEFBBBF.
* Pre:            an unsigned int representing a UTF-8 character
* Post:           boolean true or false
* Functions used: standard library functions and is1butf8(), etc.
* Includes:       stdio.h, stdlib.h, ctype.h stdbool.h
* Used in:                                                 */

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's and #define's should be bracketed by INFDEFINE's
   #include's that are not used should be removed
*/

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
bool  is1butf8(unsigned int); // returns true if 1-byte UTF-8
bool  is2butf8(unsigned int); // returns true if 2-byte UTF-8
bool  is3butf8(unsigned int); // returns true if 3-byte UTF-8
bool  is4butf8(unsigned int); // returns true if 4-byte UTF-8

bool isspaceu(unsigned int v) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   if( is1butf8(v) ) {
      if( isspace(v) ) { // using ctype.h isspace() function
         return(true);   // includes space, tab, line feed, etc.
      } else {
         return(false);
      }
   }
   /* List of Unicode space characters in
      Unicode 5.0 Standard, p. 206
      Table 6-2. Unicode Space Characters
      U+0020	20	SPACE
      U+00A0	C2A0	NO-BREAK SPACE
      U+1680	E19A80	OGHAM SPACE MARK
      U+180E	E1A08E	MONGOLIAN VOWEL SEPARATOR
      U+2000	E28080	EN QUAD
      U+2001	E28081	EM QUAD
      U+2002	E28082	EN SPACE
      U+2003	E28083	EM SPACE
      U+2004	E28084	THREE-PER-EM SPACE
      U+2005	E28085	FOUR-PER-EM SPACE
      U+2006	E28086	SIX-PER-EM SPACE
      U+2007	E28087	FIGURE SPACE
      U+2008	E28088	PUNCTUATION SPACE
      U+2009	E28089	THIN SPACE
      U+200A	E2808A	HAIR SPACE
      U+202F	E280AF	NARROW NO-BREAK SPACE
      U+205F	E2819F	MEDIUM MATHEMATICAL SPACE
      U+3000	E38080	IDEOGRAPHIC SPACE */
   if( is2butf8(v) ) {
      if ( (v == 0xC2A0) ) { // U+00A0 nonbreaking space
         return(true);
      } else {
         return(false);
      }
   }
   if( is3butf8(v) ) {
      if ( (v == 0xE19A80) || // U+1680 Ogham space mark
           (v == 0xE1a08E) || // U+180E Mongolian vowel separator
           (v == 0xE28080) || // U+2000	EN QUAD
           (v == 0xE28081) || // U+2001	EM QUAD
           (v == 0xE28082) || // U+2002	EN SPACE
           (v == 0xE28083) || // U+2003 EM SPACE
           (v == 0xE28084) || // U+2004 THREE-PER-EM SPACE
           (v == 0xE28085) || // U+2005 FOUR-PER-EM SPACE
           (v == 0xE28086) || // U+2006 SIX-PER-EM SPACE
           (v == 0xE28087) || // U+2007 FIGURE SPACE
           (v == 0xE28088) || // U+2008 PUNCTUATION SPACE
           (v == 0xE28089) || // U+2009 THIN SPACE
           (v == 0xE2808A) || // U+200A HAIR SPACE
           (v == 0xE280AF) || // U+202F NARROW NO-BREAK SPACE
           (v == 0xE2819F) || // U+205F MEDIUM MATHEMATICAL SPACE
           (v == 0xE38080) ) { // U+3000 IDEOGRAPHIC SPACE
         return(true);
      } else {
         return(false);
      }
   }
   if( is4butf8(v) ) {
      // no 4-byte characters are considered spaces
      return(false);
   }
   fprintf(stderr, "%x is not a UTF-8 characeter\n", v);
   // exit(EXIT_FAILURE);
   return(false);
} // end isspaceu()

