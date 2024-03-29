#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <stdbool.h> // needed for is1butf8() etc.

/***************************************************************
char* fgetsu(char*, int, FILE *)
This function takes as its arguments a pointer to a buffer,
an int n, and an open FILE* pointer to a file stream. It copies
up to n-1 UTF-8 characters (not bytes!) from the file stream,
and writes them to the char* buffer. If the function encounters
a newline or an end of file before reaching n-1 characters,
it only writes the characters read up to that point. However,
blank lines are preserved. This function also terminates the
buffer by appending a null character to it.
It is the responsibility of the calling function to ensure
that the buffer can accomodate all the UTF-8 characters that
are written to it.
Pre:            a pointer to a buffer, an int, and a FILE*
                pointer to a file stream
Post:           a pointer to the buffer
Functions used: standard library functions and is1butf8(), etc.
Includes:       stdio.h, stdlib.h, stdbool.h
Used in:                                                 */

/***************************************************************
*                      NEEDED ENHANCEMENTS                     *
***************************************************************/


/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
int   fgetu(FILE *);        // gets UTF-8 character from FILE *
bool  is1butf8(unsigned int); // returns true if 1-byte UTF-8
bool  is2butf8(unsigned int); // returns true if 2-byte UTF-8
bool  is3butf8(unsigned int); // returns true if 3-byte UTF-8
bool  is4butf8(unsigned int); // returns true if 4-byte UTF-8

char * fgetsu(char * buffer, int max, FILE * fp) {
   if( feof(fp) ) { // found EOF before any characters were read
      return(NULL);
   }
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // int i; // no. of bytes FILE pointer advances while reading
          // in loop through FILE* pointer
   unsigned short int j; // no. of bytes written to buffer
   unsigned short int n; // no. of characters written to buffer
   unsigned int u;  // holds return value of fgetu()
   // loop through FILE* stream, putting each UTF-8 character
   // into the unsigned int u; do not exceed max-1 characters
   for(j = 0, n = 0; (u = fgetu(fp)) && (n < max); ) {
      if( is1butf8(u) ) { // u is a 1-byte UTF-8 character
         buffer[j] = (unsigned char) ((u & 0x000000FF)      );
         if(buffer[j] == '\n' || buffer[j] == '\r') {
	    if(j != 0) { // if not first character on line
               // remove end-of-line characters & terminate buffer
               buffer[++j] = '\0';
   	       n++;
               return(buffer);
            } else {
               buffer[0] = '\n';
               buffer[1] = '\0';
               return(buffer);
            }
         }
	 ++j;
	 continue;
      }
      if( is2butf8(u) ) { // u is a 2-byte UTF-8 character
         buffer[j++] = (unsigned char) ((u & 0x0000FF00) >>  8),
         buffer[j++] = (unsigned char) ((u & 0x000000FF)      );
	 n++;
	 continue;
      }
      if( is3butf8(u) ) { // u is a 3-byte UTF-8 character
         buffer[j++] = (unsigned char) ((u & 0x00FF0000) >> 16),
         buffer[j++] = (unsigned char) ((u & 0x0000FF00) >>  8),
         buffer[j++] = (unsigned char) ((u & 0x000000FF)      );
	 n++;
	 continue;
      }
      if( is4butf8(u) ) { // u is a 4-byte UTF-8 character
         buffer[j++] = (unsigned char) ((u & 0xFF000000) >> 24),
         buffer[j++] = (unsigned char) ((u & 0x00FF0000) >> 16),
         buffer[j++] = (unsigned char) ((u & 0x0000FF00) >>  8),
         buffer[j++] = (unsigned char) ((u & 0x000000FF)      );
	 n++;
	 continue;
      }
   }
}

