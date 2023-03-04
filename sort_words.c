#include <stdio.h>
#include <stdlib.h> // needed for qsort()
#include <string.h> // needed for strcmp()
#include <locale.h> // needed for setlocale()

/* This function accepts a list of words
   in the form of an array of pointers
   to strings and sorts it in the order
   determined by a custom comparison
   function strptrcmp().
   sort_words() takes as its arguments
   an array of pointers to strings;
   a size_t number giving the size of
   the array (if an array is passed its
   size cannot be calculated by the
   receiving function); and 
   a string defining LC_COLLATE
   Here the custom comparison function
   strptrcmp() puts all words in
   lowercase before sorting.
   This program was adapted from a program
   in the following book:
   Prinz, Peter, and Tony Crawford. 2016.
   C in a Nutshell, 2nd edn., pp. 538-539.
   Beijing-Boston-Farnham-Sebastopol-Tokyo: O'Reilly. */

/***************************************************************
*                      FUNCTION DECLARATIONS                   *
***************************************************************/
int strptrcmp(const void *, const void *); // custom comparison
unsigned int getu(char *, int *);     // get UTF-8 from string
unsigned int toloweru(unsigned int);  // UTF-8-aware tolower()
char * utf8cat(char *, unsigned int); // append UTF-8 to string

/***************************************************************
*                         MAIN FUNCTION                        *
***************************************************************/
char * sort_words(char * words[ ], size_t n, char * my_locale) {
   char * current_locale = setlocale(LC_COLLATE, my_locale);
   if(!current_locale) {
      fprintf (stderr, "locale is not set!\n");
   }
   // qsort does not depend on locale,
   // but strcoll() in strptrcmp() does
   qsort(words, n, sizeof(char *), strptrcmp);
}

/***************************************************************
*                      FUNCTION DEFINITIONS                    *
***************************************************************/
/***************************************************************
This function compares two strings by reference.
strptrcmp()
Pre:            Pointers to pointers of two NULL-terminated
                strings are passed to this function by qsort().
		These pointers to pointers are dereferenced
		to obtain a char *.
Post:           If the strings are equal, the return value is 0;
		if s1 comes before s2, the return value is < 0 ;
		if s1 comes after s2, the return value is > 0.
Functions used: malloc(), setlocale(), getu(), utf8cat()
Includes:       stdlib.h, string.h, locale.h
Used in:        qsort();                      */
int strptrcmp(const void *sp1, const void *sp2) {
   const char * s1 = *(char **) sp1;
   const char * s2 = *(char **) sp2;
   // s1 and s2 are const, so make copies
   // this copy is to eliminate const qualifier
   char * s1_copy = malloc(strlen(s1) + 1);
   strcpy(s1_copy, s1);
   char * s2_copy = malloc(strlen(s2) + 1);
   strcpy(s2_copy, s2);
   // this copy to hold the processed UTF-8 characters
   char * s1u = malloc(strlen(s1) + 1);
   char * s2u = malloc(strlen(s2) + 1);
   unsigned int u; // will hold each UTF-8 character

   // get each UTF-8 character from s1, tolower() it,
   // and put the UTF-8 character into string copy s1u.
   for(int i = 0; (u = getu(s1_copy, &i)) != '\0'; i++) {
      utf8cat(s1u, toloweru(u));
   }
   // get each UTF-8 character from s2, tolower() it,
   // and put the UTF-8 character into string copy s2u.
   for(int i = 0; (u = getu(s2_copy, &i)) != '\0'; i++) {
      utf8cat(s2u, toloweru(u));
   }
   // return comparison of s1u and s2u based on locale
   return(strcoll(s1u, s2u));
}; // end strptrcmp()

