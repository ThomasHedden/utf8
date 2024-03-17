#include <stdio.h>
#include <stdbool.h> // needed for boot data type
#include <ctype.h>   // needed for isprint()

/* This function takes as its argument an
   unsigned int. It returns true if the
   unsigned int represents a valid UTF-8
   character that is printable. This
   function assumes that all 2-byte,
   3-byte, and 4-byte UTF-8 characters
   are printable. Otherwise, this function
   returns false.
   Written by Thomas Hedden 17 March 2024 */

bool is1butf8(unsigned int);
bool is2butf8(unsigned int);
bool is3butf8(unsigned int);
bool is4butf8(unsigned int);

bool isprintu(unsigned int u) {
   if((is1butf8(u) && isprint(u)) ||
       is2butf8(u)                ||
       is3butf8(u)                ||
       is4butf8(u)) {
      return(true);
   } else {
      return(false);
   }
}
