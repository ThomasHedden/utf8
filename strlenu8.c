#include <stdio.h>
#include <stdlib.h> // needed for exit

/***************************************************************
strlenu8(char *)
Pre:            This function takes a pointer to a UTF-8 string.
Post:           This function returns an int giving the number
                of UTF8 characters (not bytes!) in the string.
	       	The characters may be 1-byte, 2-byte, 3-byte,
	       	or 4-byte characters.
Functions used: getu() and standard library functions
Includes:       stdio.h and stdlib.h (for exit())
Used in:        main()                                         */

int getu(char *, int *);

size_t strlenu8(char * input_string) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   unsigned int u;   // holds value of a UTF-8 character
   unsigned short int string_length = 0; // no. of UTF-8 chars

   // loop through all characters in string
   for(int i = 0; (u = getu(input_string, &i)) != '\0'; i++) {
      string_length++;
      if(u == 0xEFBBBF) { // found UTF-8 byte order mark
         string_length--; // don't count BOM in length
         fprintf(stderr, "UTF-8 BOM not included in count\n");
      }
   }
   return((size_t) string_length);
}; /* end strlenu8() */

