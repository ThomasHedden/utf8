#include <stdio.h>
#include <stdbool.h> // needed for is1butf8(), etc.
#include <stdlib.h>  // needed for exit()
#include <string.h>  // needed for strlen()

/***************************************************************
utf8cat(char *, unsigned int) {
This function takes an unsigned int representing a UTF-8
character (the 2nd parameter) and appends the UTF-8 character
to the end of the string that is referenced by the char *
parameter (the 1st parameter). The first byte of the UTF-8
character overwrites the NULL character at the end of the
string.
If the UTF-8 character is a BOM, it is ignored.
It returns a pointer to the string to which the UTF-8
character has been appended.
It is the responsibility of the calling function to ensure
that the string has been allocated enough memory to allow
the UTF-8 character to be appended to the string.
Pre:            a NULL-terminated string and an unsigned int
                representing a UTF-8 character to be appended
		to the string.
Post:           the unsigned int representing the UTF-8
                character is appended to the end of the string,
		overwriting the NULL character at the end of
		the string. The function returns the pointer
		to this string.
		The UTF-8 character cannot be the UTF-8 BOM.
Functions used: standard library functions and is1butf8(), etc.
Includes:       stdio.h, stdbool.h, stdlib.h, string.h
Used in:        main(); ()                                   */

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
bool is1butf8(unsigned int);
bool is2butf8(unsigned int);
bool is3butf8(unsigned int);
bool is4butf8(unsigned int);
bool isutf8bom(unsigned int);

/***************************************************************
*                         MAIN FUNCTION                       *
***************************************************************/
char * utf8cat(char * str, unsigned int u) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // find end of string
   unsigned int end = strlen(str);
   if( is1butf8(u) ) {
      str[end++] = (char) (u & 0x000000FF);
      str[end] = '\0';
      return(str);
   }
   if( is2butf8(u) ) {
      str[end++] = (char) ((u & 0x0000FF00) >>  8),
      str[end++] = (char) ((u & 0x000000FF)      );
      str[end] = '\0';
      return(str);
   }
   if( is3butf8(u) ) {
      if(isutf8bom(u)) { // do not append UTF-8 BOM
         fprintf(stderr, "ignoring UTF-8 BOM 0xEFBBBF\n");
         str[end] = '\0';
         return(str);
      }
      str[end++] = (char) ((u & 0x00FF0000) >> 16),
      str[end++] = (char) ((u & 0x0000FF00) >>  8),
      str[end++] = (char) ((u & 0x000000FF)      );
      str[end] = '\0';
      return(str);
   }
   if( is4butf8(u) ) {
      str[end++] = (char) ((u & 0xFF000000) >> 24),
      str[end++] = (char) ((u & 0x00FF0000) >> 16),
      str[end++] = (char) ((u & 0x0000FF00) >>  8),
      str[end++] = (char) ((u & 0x000000FF)      );
      str[end] = '\0';
      return(str);
   }
   fprintf(stderr, "0x%X is not a UTF-8 character\n", u);
   fprintf(stderr, "String %s unchanged\n", str);
   return(str);
}

