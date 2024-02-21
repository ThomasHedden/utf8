#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false
#include <stdlib.h>  // needed for exit

/***************************************************************
bool is3b_Ucp(unsigned int)
Pre:            This function takes an unsigned int representing
                a Unicode code point.
Post:           This function returns boolean true if this
                unsigned int is a Unicode code point that
		requires 3 bytes to be encoded in the UTF-8
		encoding scheme. Otherwise, the return value
                is false.
		If the Unicode code point falls within the
		range reserved for high surrogates or low
	       	surrogates, this function prints a message
	       	to that effect and returns false. The reason
		why is that UTF-8 sequences representing
	       	surrogate code points are illegal.
		Valid ranges have been determined as the
		intersection of the ranges for UTF-8 encoding
		in Table 3-6 UTF-8 Bit Distribution and the
	       	ranges for surrogates given in D71 D72 D73 D74
	       	of section 3.8 Surrogates
		in The Unicode 5.0 Standard.
Functions used: only standard library functions
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:                                                       */
bool is3b_Ucp(unsigned int ucp) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "%s: sizeof(int) is not 4!\n", __func__);
      exit(EXIT_FAILURE);
   }
   if(ucp < 0x00000800) { // below 3 byte UTF-8 range
     return false;
   }
   if(ucp > 0x0000FFFF) { // above 3 byte UTF-8 range
     return false;
   }
   // Unicode code point that is high enough to
   // require 3 bytes when encoded in UTF-8 and
   // is below the high surrogate range, so it
   // falls within basic multilingual plane;
   if( (ucp >= 0x00000800) && (ucp <= 0x0000D7FF) ) {
      return true;
   }
   // Unicode code point that is high enough to
   // require 3 bytes when encoded in UTF-8 and
   // that falls with the high surrogate range
   // of a non-BMP character
   if( (ucp >= 0x0000D800) && (ucp <= 0x0000DBFF) ) {
      fprintf(stderr, "high surrogate\n");
      return false;
   }
   // Unicode code point that is high enough to
   // require 3 bytes when encoded in UTF-8 and
   // that falls with the low surrogate range
   // of a non-BMP character
   if( (ucp >= 0x0000DC00) && (ucp <= 0x0000DFFF) ) {
      fprintf(stderr, "low surrogate\n");
      return false;
   }
   // Unicode code point that is high enough to
   // require 3 bytes when encoded in UTF-8 and
   // that also falls above the low surrogate
   // range of a non-BMP character, so it is
   // within the basic multilingual plane
   if( (ucp >= 0x0000E000) && (ucp <= 0x0000FFFF) ) {
      return true;
   }
   // this should be impossible, but just in case
   return false;
}; /* end is3b_Ucp() */

