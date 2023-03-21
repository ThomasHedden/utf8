#include <stdio.h>
#include <stdlib.h>  // needed for exit(), malloc(,) and qsort()
#include <string.h>  // strlen(), strcpy(), strcat(), strcoll()
#include <stdbool.h> // needed for isutf8bom()
#include <locale.h>  // needed for setlocale()

/***************************************************************
                             README
 This program sorts the characters in a UTF-8 string passed to
 it as the first parameter according to the value of LC_COLLATE
 that is passed to it as the second parameter.
 It does this by putting each UTF-8 character into its own
 separate string and putting all string pointers into an array
 of pointers to strings and then using qsort() to sort the
 array of strings.
 The qsort() function was adapted from a program in the
 following book:
 Prinz, Peter, and Tony Crawford. 2016.
 C in a Nutshell, 2nd edn., pp. 538-539.
 Beijing-Boston-Farnham-Sebastopol-Tokyo: O'Reilly.
 The locale is set as follows (using French as an example):
 char * locale_to_use = setlocale(LC_COLLATE, "fr_FR.UTF-8");
 This locale must be in the list returned by the command:
 $ locale -a
 If the desired locale is not in this list, first check
 whether the locale is supported by checking this list:
 /usr/share/i18n/SUPPORTED
 If the locale is in the supported list, it can be added
 using this command (run as root):
 # locale-gen <locale name>
 The C locale is set as follows: 
 char * locale_to_use = setlocale(LC_COLLATE, NULL);
 The system locale is set as follows: 
 char * locale_to_use = setlocale(LC_COLLATE, "");
***************************************************************/

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
int          strptrcmp(const void *, const void *);
unsigned int getu(char *, int *);
char *       utf8cat(char *, unsigned int);
size_t       strlenu8(char *);
bool         isutf8bom(unsigned int);
int          putuchar(unsigned int);
char *       prepend_utf8_bom(char *);

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
char * sort_string(char * my_string, char * my_locale) {
   /************************************************************
   *                    PRELIMINARY TESTS                      *
   ************************************************************/
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sort_string(): sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // this program requires that the size of a long be 8 bytes
   if( sizeof(long int) != 8 ) { 
      fprintf(stderr, "sort_string(): sizeof(unsigned long int) is not 8!\n");
      exit(EXIT_FAILURE);
   }
   /************************************************************
   *                  VARIABLE DECLARATIONS                    *
   ************************************************************/
   unsigned int si = 0; // iterator index in string
   unsigned int ai = 0; // iterator index in array
   // temp_string will hold each UTF-8 character
   // that is to be put in the array
   char *       temp_string = malloc( 4*sizeof(char) + 1);
   if(temp_string == NULL) {
      fprintf(stderr, "on line %d in file %s\n",
                       __LINE__, __FILE__);
      fprintf(stderr, "insufficient memory\n");
   }
   unsigned int number_of_bytes_in_string;
   unsigned int number_of_characters_in_string; // UTF-8
   unsigned int u; // holds each UTF-8 character in for loop
   bool         initial_utf8_bom_found = false;

   number_of_bytes_in_string = strlen(my_string);
   number_of_characters_in_string = strlenu8(my_string); // except BOM

   // create a char * array (an array of pointers to strings)
   // for UTF-8 characters
   char * my_array_of_single_utf8_characters[number_of_characters_in_string];

   // First need to handle UTF-8 byte order mark 0xEFBBBF.
   // If present, BOM should remain at the beginning of
   // the sorted string.
   // It is essential that the UTF-8 BOM not be included
   // in the array that is passed to qsort(). If it is
   // found in initial position, it should be omitted
   // from the array that is passed to qsort() and
   // restored after sorting. If it is found elsewhere,
   // it should be, and is, discarded.
   if( ( my_string[0] == (unsigned char) 0xEF) &&
       ( my_string[1] == (unsigned char) 0xBB) &&
       ( my_string[2] == (unsigned char) 0xBF) ) {
      initial_utf8_bom_found = true;
      fprintf(stderr, "found UTF-8 BOM 0xEFBBBF\n");
      // strlenu8() excludes BOM, which will be restored
      // after sorting, so need to increase length
      number_of_characters_in_string += 1;
      si += 3; // copying should start AFTER initial BOM
   }

   for(ai = 0; (u = getu(my_string, &si)) != '\0'; si++, ai++) {
      // non-initial UTF-8 BOM found, omit
      if(isutf8bom(u)) {
         ai--; // BOM is omitted, so also omit is place in array
	 continue;
      }
      temp_string[0] = '\0';
      utf8cat(temp_string, u);
      my_array_of_single_utf8_characters[ai] = malloc(sizeof(temp_string) + 1);
      if(my_array_of_single_utf8_characters[ai] == NULL) {
         fprintf(stderr, "on line %d in file %s\n",
                          __LINE__, __FILE__);
         fprintf(stderr, "insufficient memory\n");
      }
      strcpy(my_array_of_single_utf8_characters[ai], temp_string);
   }

   // set current locale
   char * current_locale = setlocale(LC_COLLATE, my_locale);
   if (!current_locale) {
      fprintf (stderr, "Locale is not set!\n");
   }

   // sort my_array_of_single_utf8_characters[]  
   qsort(my_array_of_single_utf8_characters, 
		   sizeof(my_array_of_single_utf8_characters) / sizeof(char*),
		   sizeof(char*),  strptrcmp);

   // The array my_array_of_single_utf8_characters[] is a local
   // variable that cannot be returned to the calling function,
   // so its contents must be copied into a string with allocated
   // memory. Its size will be the same as the original string
   // that was passed to this function.
   char * return_string = malloc(number_of_bytes_in_string + 1);
 
   // copy my_array_of_single_utf8_characters[] into return_string
   for(ai = 0; ai < number_of_characters_in_string; ai++) {
      strcat(return_string, my_array_of_single_utf8_characters[ai]); 
   }
   my_array_of_single_utf8_characters[number_of_characters_in_string + 1] = '\0'; 

   // UTF-8 BOM might have been omitted above. If so, it should be
   // restored here.
   if(initial_utf8_bom_found) {
      prepend_utf8_bom(return_string);
   }

   return(return_string);
}

/***************************************************************/
/*                    FUNCTION DEFINITIONS                     */
/***************************************************************/
// all other functions have been put in separate files
int strptrcmp(const void *sp1, const void *sp2) {
   // Compare two strings by reference.
   // qsort() passes pointers to elements in the array,
   // which are themselves pointers to strings.
   // These pointers to pointers are dereferenced 
   // to pass pointers to strings to strcoll().
   const char * s1 = *(char **) sp1;
   const char * s2 = *(char **) sp2;
   return(strcoll(s1, s2));
}

