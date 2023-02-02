#include <stdio.h>
#include <stdlib.h>   // needed for exit()
#include <stdbool.h>  // needed for is1butf8() etc.
#include <string.h>   // needed for strlen() and for strcspn()

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program tests the funtion decode_utf8(), which takes
   a Unicode character in the form of the hexadecimal value of
   a UTF-8 character and converts it into the corresponding
   Unicode code point in the form U+(hex value).
   For example, 0xD096, which corresponds to Russian capital
   letter Ж (ZH) is converted into Unicode code point U+0416.
   Written by Thomas Hedden April 2021.
   Modified in February 2023.
*/

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's and #define's should be bracketed by INFDEFINE's
   #include's that are not used should be removed
   #define's that are not used should be removed
   variables that are not used should be removed
   functions that are not used should be removed
   allocated memory should be freed
   files should be closed
   testing statements should be removed
   commented out code should be removed
*/

/***************************************************************/
/*                     #DEFINE STATEMENTS                      */
/***************************************************************/
#define MAXARRAY  80       /* size of array into which words
                              (one word per line expected)
                              from input files are placed      */
#define MXFNSZ   256       /* maximum length of filenames (using
                              Second Extended Filesystem)      */
#define MXSZVAR   32       /* maximum size of ANSI C variables,
                              including '\0'                   */

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
void  getinputfile(unsigned char []); // prompt for file name
FILE* open_file(char*, char*); 
int   fgetu(FILE *);      // gets UTF-8 character from FILE *
bool  isb1ofu8(unsigned char);
bool  is1butf8(unsigned int);
bool  is2butf8(unsigned int);
bool  isb1of2b(unsigned int);
bool  istbutf8(unsigned int);
bool  is3butf8(unsigned int);
bool  isb1of3b(unsigned int);
bool  is4butf8(unsigned int);
bool  isb1of4b(unsigned int);
bool  isspaceu(unsigned int);
bool  isutf8bom(unsigned int);
// converts UTF-8 byte to Unicode code point
unsigned long int decode_utf8(unsigned int); 

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
int main(int argc, char *argv[]) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   unsigned char i_file_name[MXFNSZ]; // holds input file name
   FILE      *infp; // pointer to input stream of type FILE
   int i;                             // loop iterator
   unsigned char word_buffer[MXFNSZ]; // array to hold word
   unsigned int u;  // holds UTF-8 value from fgetu()

   /************************************************************
   *                       CHECK INPUT                         *
   ************************************************************/
   if( argc == 1 ) { // no input file name entered ...
      getinputfile(i_file_name); // ... so prompt for it
   } else { // input file name entered, so put in it i_file_name
      for(i = 0; i < MXFNSZ && argv[1][i] != '\0'; i++) {
         i_file_name[i] = argv[1][i];
      }
      i_file_name[i] = '\0';
   }

   /************************************************************
   *                        OPEN FILE                          *
   ************************************************************/
   // open input file to be processed
   infp = open_file(i_file_name, "r");

   /************************************************************
   *   GET INPUT FROM INPUT STREAM & OUTPUT TO OUTPUT STREAM   *
   ************************************************************/
   while( u = fgetu(infp) ) { // traverse stream
      // if found end of file
      if( feof(infp) || u == EOF ) {
         break; // found EOF, so stop, don't print anything
      }
      if( isspaceu(u) ) { // print new line instead of space
         fprintf(stdout, "\n");
         continue;
      }
      if( u == 0x0A ) { // print real new line, not its code point
         fprintf(stdout, "\n");
         continue;
      }
      if( u == 0x0d ) { // ignore carriage returns
         continue;
      }
      // found UTF-8 BOM, ignore it instead of converting it
      if(isutf8bom(u)) {
         // If it is desired to convert UTF-8 BOM 0xEFBBBF
         // to big-endian UTF-16 BOM 0xFEFF, uncomment the
         // following three lines:
         // fprintf(stdout, "U+");
         // fprintf(stdout, "%#04x", (unsigned int) (decode_utf8(u) >> 32));
         // fprintf(stdout, " ");
         continue;
      }
      fprintf(stdout, "U+");
      fprintf(stdout, "%#04x", (unsigned int) decode_utf8(u));
      fprintf(stdout, " ");
      continue;
   }
   // fprintf(stdout, "\n");

   /**********************************************************
   *                         CLOSE FILES                     *
   **********************************************************/
   // close the file stream of file to process fclose(infp); return(0);
   fclose(infp);

   return(0);
}

/***************************************************************/
/*                    FUNCTION DEFINITIONS                     */
/***************************************************************/
// all other functions have been put in separate files

