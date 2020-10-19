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
void putu(unsigned int u) {
   if( is1butf8(u) || is2butf8(u) || is3butf8(u) || is4butf8(u) ) {
      fprintf(stdout, "%c%c%c%c", (char) ((u & 0xFF000000) >> 24),
                                  (char) ((u & 0x00FF0000) >> 16),
                                  (char) ((u & 0x0000FF00) >>  8),
                                  (char) ((u & 0x000000FF)      ));
   }
}

