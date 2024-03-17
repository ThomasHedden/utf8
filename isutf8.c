#include <stdio.h>
#include <stdbool.h>

/* This function takes as its argument an
   unsigned int. It returns true if the
   unsigned int represents a valid UTF-8
   character. Otherwise, it returns false.
   Written by Thomas Hedden March 2024. */

bool is1butf8(unsigned int);
bool is2butf8(unsigned int);
bool is3butf8(unsigned int);
bool is4butf8(unsigned int);

bool isutf8(unsigned int u) {
   if(is1butf8(u) ||
      is2butf8(u) ||
      is3butf8(u) ||
      is4butf8(u)  ) {
      return(true);
   } else {
      return(false);
   }
}
