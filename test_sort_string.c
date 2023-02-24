#include <stdio.h>
#include <stdlib.h>   // needed for exit()
#include <stdbool.h>  // needed for is1butf8() etc.
#include <string.h>   // needed for strlen() and for strcspn()
#include <locale.h>   // needed for set_locale()

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This program tests the funtion sort_string(), which sorts
   a UTF-8 string that is passed to it as the first parameter
   according to the value of LC_COLLATE that is passed to it
   as the second parameter.
   The locale is set as follows (using French as an example):
   char * locale_to_use = setlocale(LC_COLLATE, "fr_FR.UTF-8");
   This locale must be in the list returned by the command:
   $ locale -a
   If the desired locale is not in the list, first check
   whether the locale is supported by checking this list:
   /usr/share/i18n/SUPPORTED
   If the locale is in the supported list, it can be added
   using this command (run as root):
   # locale-gen <locale name>
   The C locale is set as follows: 
   char * locale_to_use = setlocale(LC_COLLATE, NULL);
   The system locale is set as follows: 
   char * locale_to_use = setlocale(LC_COLLATE, "");
   Written by Thomas Hedden in February 2023.
   Modified March 2023.
*/

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's and #define's should be bracketed by INFDEFINE's
   #define's that are not used should be removed
   allocated memory should be freed
   files should be closed
*/

/***************************************************************/
/*                     #DEFINE STATEMENTS                      */
/***************************************************************/
#define MAXARRAY  80       /* size of array into which words
                              (one word per line expected)
                              from input files are placed      */
#define MXFNSZ   256       /* maximum length of filenames (using
                              Second Extended Filesystem)      */

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
void   getinputfile(unsigned char []); // prompt for file name
FILE*  open_file(char*, char*); 
int    fgetu(FILE *);      // gets UTF-8 character from FILE *
char*  fgetsu(char *, int, FILE *); // gets string from FILE *
bool   isb1ofu8(unsigned char);
bool   is1butf8(unsigned int);
bool   is2butf8(unsigned int);
bool   isb1of2b(unsigned int);
bool   istbutf8(unsigned int);
bool   is3butf8(unsigned int);
bool   isb1of3b(unsigned int);
bool   is4butf8(unsigned int);
bool   isb1of4b(unsigned int);
bool   isspaceu(unsigned int);
bool   isutf8bom(unsigned int);
char * sort_string(char *, char *);

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
int main(int argc, char *argv[]) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // ... and that the size of a long int be 8 bytes
   if( sizeof(long int) != 8 ) { 
      fprintf(stderr, "sizeof(unsigned long int) is not 8!\n");
      exit(EXIT_FAILURE);
   }
   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   unsigned char i_file_name[MXFNSZ]; // holds input file name
   FILE      *infp; // pointer to input stream of type FILE
   int           i;                   // loop iterator
   unsigned char word_buffer[MXFNSZ]; // array to hold word
   unsigned int  u; // holds UTF-8 value from fgetu()

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
   while( fgetsu(word_buffer, MAXARRAY, infp) != NULL ) {
      // word_buffer might have EOL, so remove before printing
      word_buffer[strcspn(word_buffer, "\r\n")] = 0;

      // the following will cause use of system locale
      char * locale_to_use = setlocale(LC_COLLATE, "");
      if (locale_to_use == NULL) {
         fprintf (stderr, "Locale is not set!\n");
      }
      fprintf(stderr, "using locale %s\n", locale_to_use);

      // don't print blank lines: since EOL were removed,
      // blank lines will contain only '\0'.
      if((word_buffer[0] != '\0')) {
         fprintf(stdout, "original word is:\n");
         fprintf(stdout, "%s\n", word_buffer);
         fprintf(stdout, "sorted word is:\n");
         fprintf(stdout, "%s\n",
            sort_string(word_buffer, locale_to_use));
      }
      free(locale_to_use);
   }

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

