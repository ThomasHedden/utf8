#include <stdio.h>
#include <stdlib.h> // needed for exit()

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program was written by Thomas Hedden on 06 July 2020.
   Revised 30 December 2021 to allow for more than a single
   character.
   It takes a command line argument of UTF-8 characters and
   prints out the equivalent decimal (base 10) numbers to stdout. */

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's and #define's should be bracketed by INFDEFINE's
   need to allow more than one UTF-8 character to be input
   without resulting in stack crashing. See utox.c for code. */

/***************************************************************/
/*               STRUCT DECLARATIONS AND TYPEDEFS              */
/***************************************************************/
/* to make it quicker to declare a nice small data type        */
typedef unsigned int UNT; // to hold value of UTF-8 char
typedef short int    IT;  // for loop iterators

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
UNT getu(char *, int *);
// UNT decode_utf8(unsigned int); // returns U code point of
                               // input hex value

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
   UNT u = 0;
   IT i; // loop iterator
   IT j; // loop iterator

   if(argc < 2) { // check input
      // print usage message
      fprintf(stderr, "Usage: %s UTF-8 text\n", argv[0]);
      exit(EXIT_FAILURE);
   } else {
      // cycle through each positional parameter
      for(IT i = 1; argv[i] != NULL; i++) {
         // cycle through each UTF-8 character in this positional parameter
         for(int j = 0; (u = getu(argv[i], &j)) != '\0'; j++) {
            // calculate value of corresponding unsigned int
            fprintf(stdout, "%d", u);
	    fprintf(stdout, " ");
         }
	 fprintf(stdout, "\n");
      }
   }
   return(0);
}

/***************************************************************/
/*                    FUNCTION DEFINITIONS                     */
/***************************************************************/
// all functions have been put in separate files

