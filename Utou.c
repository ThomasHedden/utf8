#include <stdio.h>
#include <stdlib.h> // needed for exit()

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program takes a Unicode code point either in the format
   U+<hex value> or u+<hex value> or U<hex value> or u<hexvalue>
   or simply <hex value> and outputs the character corresponding
   to that Unicode code point encoded in UTF-8.
   This program does not output Unicode code points that
   represent high surrogates or low surrogates or that are
   greater than the allowed range for Unicode code points.
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
#define MXFNSZ   256       /* maximum length of filenames (using
                              Second Extended Filesystem)      */

/***************************************************************/
/*               STRUCT DECLARATIONS AND TYPEDEFS              */
/***************************************************************/
/* to make it quicker to declare a nice small data type        */
typedef unsigned short int IT;
typedef unsigned int UNT;

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
int putuchar(unsigned int); // output UTF-8 character
UNT encode_utf8(UNT); // convert Unicode code point to UTF-8
unsigned int remove_ucp_prefix(char *);

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
int main(int argc, char *argv[]) {
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   unsigned char input_buffer[MXFNSZ]; // array to hold input
   UNT ucp; // Unicode code point
   IT  i;   // loop iterator

   /************************************************************
   *                       CHECK INPUT                         *
   ************************************************************/
   if( argc == 2 ) {
      // get input from command line, remove "U+" prefix,
      // and convert to unsigned int
      ucp = remove_ucp_prefix(argv[1]);
   } else {
      fprintf(stderr, "Usage: %s Unicode code point\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // before outputting, ensure that Unicode code point
   // falls within permissible range
   // BMP range below range reserved for surrogates
   if( (ucp >= 0x0000) && (ucp <= 0xD7FF) ) {
      // output corresponding UTF-8 character
      putuchar(encode_utf8(ucp));
      fprintf(stdout, "\n");
      return(0);
   }
   // BMP range above range reserved for surrogates
   if( (ucp >= 0xE000) && (ucp <= 0xFFFF) ) {
      // output corresponding UTF-8 character
      putuchar(encode_utf8(ucp));
      fprintf(stdout, "\n");
      return(0);
   }
   // range that would require surrogates in UTF-16;
   // however no surrogates are used in UTF-8
   if( (ucp >= 0x10000) && (ucp <= 0x10FFFF) ) {
      // output corresponding UTF-8 character
      putuchar(encode_utf8(ucp));
      fprintf(stdout, "\n");
      return(0);
   }
   // range of Unicode code points reserved for
   // high surrogates in UTF-16; UTF-8 does not
   // use surrogates, however UTF-8 sequences
   // representing values in this range are illegal
   if( (ucp >= 0xD800) && (ucp <= 0xDBFF) ) { // HS range
      fprintf(stderr, "0x%X is in high surrogate range!\n", ucp);
      exit(EXIT_FAILURE);
   }
   if( (ucp >= 0xDC00) && (ucp <= 0xDFFF) ) { // LS range
      fprintf(stderr, "0x%X is in low surrogate range!\n", ucp);
      exit(EXIT_FAILURE);
   }
   if( ucp > 0x10FFFF ) { // outside of valid Unicode range
      fprintf(stderr, "outside of range for Unicode code points!\n");
      exit(EXIT_FAILURE);
   }

   return(0);
}

/***************************************************************/
/*                    FUNCTION DEFINITIONS                     */
/***************************************************************/
// all functions have been put in seprate files

