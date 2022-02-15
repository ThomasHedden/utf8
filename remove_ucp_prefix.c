#include <stdio.h>
#include <stdlib.h> // needed for exit()

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This function takes a string that represents a Unicode code
   point either in the format U+<hex value> or u+<hex value>
   or U<hex value> or u<hex value> or simply <hex value>,
   and returns the unsigned int corresponding to that string
   without the prefix "U+" or "u+" or "U" or "u".
   It performs essentially the same function as strtoul(),
   but first it removes "U+" or "u+" or "U" or "u", if present.
   Written by Thomas Hedden February 2022.
*/

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's and #define's should be bracketed by INFDEFINE's
*/

/***************************************************************/
/*                     #DEFINE STATEMENTS                      */
/***************************************************************/
#define MXFNSZ 50 // size of buffer to hold Unicode code point

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
unsigned int remove_ucp_prefix(char * ucp_string) {
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   unsigned char input_buffer[MXFNSZ]; // buffer to hold input
   unsigned int ucp;                   // Unicode code point
   short int i, j;                     // loop iterators

   if( ucp_string == NULL ) { // no string was passed
      return(0); // return 0 to prevent memory faults
   }

   // This loop removes "U+" or "u+", etc., if present.
   // Otherwise, the passed string is simply copied
   for(i = 0, j = 0; ucp_string[i] != '\0'; i++, j++) {
      // check for leading "U" or "u", if present
      if( ucp_string[i] == 'U' || ucp_string[i] == 'u') {
         i++; // skip over "U" or "u", if present
         // next check for "+", if present,
	 // but only if preceded by "U" or "u"
         if( ucp_string[i] == '+' ) {
            i++; // also skip over "+", if present
	 }
      }
      input_buffer[j] = ucp_string[i];
   }

   // convert input_buffer to unsigned int
   // to get Unicode code point ucp
   ucp = strtoul(input_buffer, NULL, 16); 

   return(ucp);
}

/***************************************************************/
/*                    FUNCTION DEFINITIONS                     */
/***************************************************************/
// all functions have been put in seprate files

