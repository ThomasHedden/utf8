#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <stdbool.h> // needed for is1butf8(), etc.

/***************************************************************
putuchar(unsigned int) {
This function takes an unsigned int representing a UTF-8 character
and writes the UTF-8 character to stdout.
It returns the value of the UTF-8 character written.
Pre:            an unsigned int representing a UTF-8 character
                to be written to stdout
Post:           an unsigned int representing the UTF-8 character
                written to stdout
Functions used: standard library functions and is1butf8(), etc.
Includes:       stdio.h, stdlib.h, stdbool.h
Used in:        main(); ()                                     */


/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
bool is1butf8(unsigned int);
bool is2butf8(unsigned int);
bool is3butf8(unsigned int);
bool is4butf8(unsigned int);

/***************************************************************
*                     FUNCTION DEFINITIONS                    *
***************************************************************/
int putuchar(unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   if( is1butf8(u) ) {
      fprintf(stdout, "%c",       (char) ((u & 0x000000FF)      ));
      return(u);
   }
   if( is2butf8(u) ) {
      fprintf(stdout, "%c%c",     (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
      return(u);
   }
   if( is3butf8(u) ) {
      fprintf(stdout, "%c%c%c",   (char) ((u & 0x00FF0000) >> 16),
                                  (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
      return(u);
   }
   if( is4butf8(u) ) {
      fprintf(stdout, "%c%c%c%c", (char) ((u & 0xFF000000) >> 24),
                                  (char) ((u & 0x00FF0000) >> 16),
                                  (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
      return(u);
   }
   fprintf(stderr, "0x%X is not a UTF-8 character\n", u);
   fprintf(stdout, "%c%c%c", (char) 0xEF, (char) 0xBF, (char) 0xBD);
   return(0xEFBFBD); // non-character
}


