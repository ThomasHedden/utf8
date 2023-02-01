#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <unistd.h>  // needed for isatty()
#include <stdbool.h> // needed for isspaceu()

#define IN       1 // inside a word
#define OUT      0 // outside a word
#define COUNTEOL 0 // include EOL characters in character count 

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program was based on the original word count program on
   p. 20 of the following book: Kernighan, Brian W., and Dennis
   M. Ritchie. 1988. The C Programming Language, 2nd edn.
   Englewood Cliffs, NJ: Prentice Hall. ISBN 0-13-110362-8
   Revised by Tom Hedden in Nov 2022 to work with UTF-8
   characters in addition to ASCII characters.
   Further modified in February 2023.
   Also can count words in a file as well as words piped
   in to stdin. */

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
FILE*        open_file(char* filename, char* mode);
unsigned int fgetu(FILE *, int *); // get next UTF-8 character
bool isspaceu(unsigned int); // any type of space
bool iseol(char); // either '\n' or '\r'

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
/* count lines, words, and characters in input */
int main(int argc, char *argv[]) {
   // this program requires that an int have 4 bytes
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }

   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   // number of lines, words, and characters
   // state is whether IN or OUT of word
   int nl, nw, nc, state;
   unsigned int u = 0; // replaces original variable c
   unsigned int v = 0; // previous value of variable u
   // name of file to open, which comes from command line 
   char * file_to_open;
   // FILE stream pointer corresponding to this file name
   FILE * fp;

   // check whether file name was provided as command line argument
   if(argc == 2) { // file name provided as command line argument
      file_to_open = argv[1];
      // open file_to_open read-only and create FILE stream from it
      fp = open_file(file_to_open, "r");
   } else {
      if(argc < 2) { // no command line argument was provided
         if (isatty(fileno(stdin))) { // stdin is connected to a terminal
            fprintf(stderr, "stdin is connected to a terminal\n");
            fprintf(stderr, "Pipe stdout to this program or enter file name\n");
            return(1);
         } else {
            fprintf(stderr, "stdin is NOT connected to a terminal\n");
            fp = stdin;
         }
      }
   }

   state = OUT;
   nl = nw = nc = 0;

   // Cycle through each positional parameter.
   // Pointer to loop iterator must be passed to fgetu()
   // to keep track of current index in string
   for(unsigned int i = 0; (u = fgetu(fp, &i)) != '\0'; i++) {
      if(u == 0xEFBBBF) { // do not count UTF-8 BOM
         fprintf(stderr, "ignoring UTF-8 BOM 0xEFBBBF\n");
      } else {
         ++nc; // count character, including EOL
      }
      if (iseol(u)) { // either '\n' or '\r'
         // as long as this EOL u was not part
	 // of a \n\r or \r\n combination ...
         if (!(u == '\n' && v == '\r') && 
             !(u == '\r' && v == '\n')) {
            ++nl; // ... increment line count
	 }
         if(!COUNTEOL) { // if EOL characters are to be omitted
            --nc;        // from character count, decrement char
         }               // count since EOL was counted above
      }
      if (isspaceu(u)) { // found a space, so we are
         state = OUT;    // OUTside of a word
      } else if (state == OUT) { // not a space, but we WERE
         state = IN; // OUTside a word and now we are INside,
         ++nw; // so this is a new word: increment no. of words
      }
      v = u; // save previous value of u for next iteration
   }

   printf("%d %d %d\n", nl, nw, nc);

   // close the file stream
   fclose(fp);

   return(0);
}

/****************************************************************
*                    FUNCTION DEFINITIONS                       *
****************************************************************/
// all functions have been put in separate files

