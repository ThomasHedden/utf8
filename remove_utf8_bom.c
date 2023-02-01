#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program takes a UTF-8 string and removes any UTF-8
   BOMs 0xEFBBBF that are found.
   Written by Thomas Hedden January 2023. */

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
unsigned int getu(char *, int *);
bool         isutf8bom(unsigned int);
char *       utf8cat(char *, unsigned int);

char * remove_utf8_bom(char * passed_string) {
   int si; // iterator for passed UTF-8 string
   unsigned int u; // holds each UTF-8 character
   char * returned_string = malloc(strlen(passed_string) + 1);
   if(returned_string == NULL) {
      fprintf(stderr, "remove_utf8_bom line 23: insufficient memory\n");
      exit(EXIT_FAILURE);
   }
   // look at each UTF-8 character in passed string
   for(si = 0; (u = getu(passed_string, &si)) != '\0'; si++) {
      if(isutf8bom(u)) {
          // if u is UTF-8 BOM 0xEFBBBF, skip it
         fprintf(stderr, "removed UTF-8 BOM 0xEFBBBF\n");
	 continue; // look at next UTF-8 character
      }
      utf8cat(returned_string, u); // otherwise, copy it
   }
   utf8cat(returned_string, '\0'); // terminate copy
   return(returned_string);
}
/****************************************************************
*                    FUNCTION DEFINITIONS                       *
****************************************************************/
// all functions have been put in separate files

