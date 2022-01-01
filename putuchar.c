#include <stdio.h>
#include <stdbool.h>

/***************************************************************
*                      NEEDED ENHANCEMENTS                     *
***************************************************************/

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
bool is1butf8(unsigned int);
bool is2butf8(unsigned int);
bool is3butf8(unsigned int);
bool is4butf8(unsigned int);

/***************************************************************
*                     FUNCTION DEFINITIONS                    *
***************************************************************/
int putu(unsigned int u) {
   unsigned short int number_of_bytes = 0;
   if( is1butf8(u) ) {
      number_of_bytes += 1;
      fprintf(stdout, "%c",       (char) ((u & 0x000000FF)      ));
   }
   if( is2butf8(u) ) {
      number_of_bytes += 2;
      fprintf(stdout, "%c%c",     (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
   }
   if( is3butf8(u) ) {
      number_of_bytes += 3;
      fprintf(stdout, "%c%c%c",   (char) ((u & 0x00FF0000) >> 16),
                                  (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
   }
   if( is4butf8(u) ) {
      number_of_bytes += 4;
      fprintf(stdout, "%c%c%c%c", (char) ((u & 0xFF000000) >> 24),
                                  (char) ((u & 0x00FF0000) >> 16),
                                  (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
   }
   return(number_of_bytes);
}




