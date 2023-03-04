#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <string.h>  // needed for strcspn()
#include <stdbool.h> // needed for iseol()

/***************************************************************
*                             README                           *
 This program tests the behavior of the function sort_words().
 This program takes argv[1] as a command-line argument which
 represents a filename. The file must contain UTF-8-encoded
 words, one word per line. The program outputs to stdout the
 list of words sorted according to the value of LC_COLLATE
 that is passed to the function sort_words(), which sorts
 words in an array of pointers to strings.
 sort_words() takes as its arguments a pointer to an array of
 pointers to strings, an int defining the length of the array
 (the length of a passed array cannot be determined in the
 receiving function), and a string defining the value of
 LC_COLLATE of the desired locale. It has return type void.
 To sort the words in a list, the list has to be converted
 into an array of pointers to strings, with each element of
 the array holding a pointer to one string. This test
 program illustrates how to do this.
 Written by Thomas Hedden February-March 2023.
***************************************************************/

/***************************************************************/
/*                     #DEFINE STATEMENTS                      */
/***************************************************************/
#define MAXARRAY 320     /* size of array into which words
                            (one word per line expected)
                            from input files are placed
                            measured in BYTES                  */
#define MXFNSZ   256     /* maximum length of filenames (using
                            Second Extended Filesystem)        */

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
FILE*        open_file(char*, char*);
short int    line_count_of(FILE *);
bool         iseol(char);
void         sort_words(char **, int, char *);
char *       fgetsu(char *, int, FILE *);
char *       fgetu(char *, int, FILE *);

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
int main(int argc, char * argv[]) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   /**********************************************************
   *                        CHECK INPUT                      *
   **********************************************************/
   if( argc != 2 ) {
      fprintf(stderr, "Usage: %s filename\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   /**********************************************************
   *                        OPEN FILES                       *
   **********************************************************/
   FILE * infp = open_file(argv[1], "r");

   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   // words[] is declared below, on line 106
   short int lines = 0; // no. of non-empty lines in file
   short int this_line; // number of line being processed
   int       i;         // loop iterator
   // holds strings on lines in file, one at a time
   char      temp_array[MAXARRAY];

   /************************************************************
   *   GET INPUT FROM INPUT STREAM & OUTPUT TO OUTPUT STREAM   *
   ************************************************************/
   // count non-empty lines in file to make it possible
   // to declare the array words[] with a fixed size (line 106)
   lines = line_count_of(infp);

   rewind(infp); // start over at beginning of file

   if(lines == 0) {
      fprintf(stderr, "blank line, nothing to do\n");
      exit(0);
   }

   if(lines == 1) {
       // MAXARRAY is in BYTES; the limit parameter of
       // fgetsu() is in UTF-8 characters, each of which
       // can be up to four bytes each, so need to divide
       // MAXARRAY by four to prevent overflow.
      fgetsu(temp_array, (MAXARRAY/4), infp);
      // remove end-of-line characters
      temp_array [strcspn(temp_array, "\r\n")] = 0;
      fprintf(stderr, "only one line, nothing to do\n");
      fprintf(stdout, "%s\n", temp_array);
      exit(0);
   }
 
   // holds pointers to strings of all lines in file
   char * words[lines];

   // Copy FILE* stream into temp_array[] line by line.
   // See note on lines 93-96 above about MAXARRAY.
   this_line = 0; // lines in file are counted from zero
   while( fgetsu(temp_array, (MAXARRAY/4), infp) != NULL ) {
      // if line is empty, skip
      if(temp_array[0] == '\n' || temp_array[0] == '\r') {
	 continue;
      } else { // if not, remove end-of-line characters
         temp_array [strcspn(temp_array, "\r\n")] = 0;
      }

      // need to copy the array representing the string
      // on each line to a string with allocated memory
      char * temp_string = malloc(strlen(temp_array) + 1);
      if(temp_string == NULL) {
         fprintf(stderr, "test_sort_words.c line 121: \
			   insufficient memory\n");
         exit(EXIT_FAILURE);
      }

      // copy array into string
      for(i = 0; i < strlen(temp_array); i++) {
         temp_string[i] = temp_array[i];
      }
      temp_string[i] = '\0';

      // copy string with allocated memory into words[i]
      words[this_line] = temp_string;

      this_line++;
      temp_array[0] = '\0'; // for next iteration
   }

   fprintf(stdout, "original word list is:\n");
   for(i = 0; i < lines; i++) {
      puts(words[i]);
   }

   sort_words(words, lines, "fr_FR.UTF-8");

   fprintf(stdout, "sorted word list is:\n");
   for(i = 0; i < lines; i++) {
      puts(words[i]);
   }

   /**********************************************************
   *                        CLOSE FILES                      *
   **********************************************************/
   fclose(infp);

   return(0);   
}

/***************************************************************
*                     FUNCTION DEFINITTIONS                    *
***************************************************************/
// all functions have been put in separate files

