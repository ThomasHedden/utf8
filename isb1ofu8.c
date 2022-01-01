#include <stdio.h>
#include <stdbool.h> // needed for bool, true, false

/***************************************************************
isb1ofu8(unsigned char)
Pre:            This function takes an unsigned char.
Post:           This function returns boolean true if this
                unsigned char is any one of these:
	        - a 1-byte UTF-8 character
		- the first byte of a 2-byte UTF-8 character
	        - the first byte of a 3-byte UTF-8 character
	        - the first byte of a 4-byte UTF-8 character;
	       	otherwise, the return value is false.
Functions used: standard library functions, is1butf8(), 
                isb1of2b(), isb1of3b(), & isb1of4b()
Includes:       stdio.h, stdbool.h, & stdlib.h (for exit())
Used in:        fgetu.c, getu.c,                           */

bool is1butf8(unsigned int);
bool isb1of2b(unsigned int);
bool isb1of3b(unsigned int);
bool isb1of4b(unsigned int);

bool isb1ofu8(unsigned char u) {
   if(is1butf8(u) || // is a 1-byte UTF-8 character
      isb1of2b(u) || // is byte 1 of a 2-byte UTF-8
      isb1of3b(u) || // is byte 1 of a 3-byte UTF-8
      isb1of4b(u)) { // is byte 1 of a 4-byte UTF-8
      return true;
   } else {
      // not the first byte of a valid UTF-8 aracter
      return false;
   }
}; /* end isb1ofu8() */

