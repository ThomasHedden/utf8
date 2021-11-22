#include <stdio.h>
#include <stdlib.h> // needed for exit()

/* This program was written by Thomas Hedden on 03 July 2020.
   It used to a command line argument of a UTF-8 character and
   printed out the equivalent hexadecimal number to stdout.
   Updated by Thomas Hedden October 26 2021 to do the same
   for an arbitrary number of strings of UTF-8 characters
   of arbitrary length instead of just one UTF-8 character. */

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
int putu(UNT);

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
int main(int argc, char *argv[]) {
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   UNT u;

   if(argc < 2) {
      // print usage message
      fprintf(stderr, "Usage: %s UTF-8 text\n", argv[0]);
      exit(1);
   } else {
      // cycle through each positional parameter
      for(IT i = 1; argv[i] != NULL; i++) {
         // cycle through each UTF-8 character in this positional parameter
         for(int j = 0; (u = getu(argv[i], &j)) != '\0'; j++) {
            // no. of bytes is variable, so print 0x explicity rather than use %#x
            fprintf(stdout, "0X");
	    // only print first two bytes if they are not zero
            if( (u & 0xFF000000) != 0 ) {
               fprintf(stdout, "%02X", (unsigned char) ((u & 0xFF000000) >> 24));
            }
            if( (u & 0x00FF0000) != 0 ) {
	       fprintf(stdout, "%02X", (unsigned char) ((u & 0x00FF0000) >> 16));
	    } 
            fprintf(stdout, "%02X", (unsigned char) ((u & 0x0000FF00) >>  8));
            fprintf(stdout, "%02X", (unsigned char) ((u & 0x000000FF)      ));
            fprintf(stdout, " ");
         }
         fprintf(stdout, "\n");
      }
   }
   return(0);
}

