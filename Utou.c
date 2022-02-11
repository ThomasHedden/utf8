#include <stdio.h>
#include <stdlib.h> // needed for exit()
#include <string.h> // needed for strcpy()

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program takes a Unicode code point either in the format
   U+<hex value> or u+<hex value> or simply <hex value>
   and outputs the character corresponding to Unicode code point
   encoded in UTF-8.
   Written by Thomas Hedden December 2021.
   Modified by Thomas Hedden February 2022 so that it does not
   output Unicode code points that represent high surrogates
   or low surrogates or that are greater than the allowed range
   for Unicode code points.
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
      // get input from command line
      strcpy(input_buffer, argv[1]);
   } else {
      fprintf(stderr, "Usage: %s Unicode code point\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // loop through every byte in command line argument argv[1]
   // byte by byte and put into buffer
   for(int i = 0; argv[1][i] != '\0'; i++) {
      // first remove "U+" or "u+", if present
      if( argv[1][i] == 'U' || argv[1][i] == 'u') {
         if( argv[1][i+1] == '+' ) {
            for(i = 0; argv[1][i] != '\0'; i++) {
               input_buffer[i] = argv[1][i+2];
            }
         }
      } 
   }

   // convert input_buffer to unsigned int
   // to get Unicode code point ucp
   ucp = strtoul(input_buffer, NULL, 16); 

   // before outputting, ensure that Unicode code point
   // falls within permissible range
   if( (ucp >= 0x0000) && (ucp <= 0xD7FF) ) { // BMP range
      // output corresponding UTF-8 character
      putuchar(encode_utf8(ucp));
      fprintf(stdout, "\n");
      return(0);
   }
   if( (ucp >= 0xE000) && (ucp <= 0xFFFF) ) { // BMP range
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
   if( (ucp >= 0xD800) && (ucp <= 0xDBFF) ) { // HS range
      fprintf(stderr, "%u is in high surrogate range!\n", ucp);
      exit(EXIT_FAILURE);
   }
   if( (ucp >= 0xDC00) && (ucp <= 0xDFFF) ) { // LS range
      fprintf(stderr, "%u is in low surrogate range!\n", ucp);
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

