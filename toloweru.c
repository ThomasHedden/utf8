#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <ctype.h>   // needed for tolower()
#include <stdbool.h> // needed for is1butf8(), etc.

/* This function takes as its argument a four-byte
   unsigned int representing a UTF-8 character, and
   attempts to return the lower-case equivalent.
   This function should work for major West European
   and East European languages, for Greek, and for
   Cyrillic. For many other languages, it returns
   the result of the ctype.h function tolower().
*/

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

unsigned int toloweru(unsigned int u) {
   // this program requires that an int have 4 bytes
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // if u is a single-byte character, use tolower()
   if( is1butf8(u) )
      return(tolower(u));
   // if u is a two-byte character, need to look at details
   if( is2butf8(u) ) {
      if ( (u >= 0xC380 && u <= 0xC396) ||  // A grave to O dieresis
           (u >= 0xC398 && u <= 0xC39E) ) { // O stroke to capital thorn
         return(u + 0x20);
      }

      // Latin extended A and B, etc.
      if ( (u >= 0xC480) && (u <= 0xC4B7) && ( !(u%2) )  || // uc even, lc odd 
           (u >= 0xC4B9) && (u <= 0xC588) && (  (u%2) )  || // uc odd, lc even
           (u >= 0xC58A) && (u <= 0xC5B7) && ( !(u%2) )  || // uc even, lc odd 
           (u >= 0xC5B9) && (u <= 0xC5BE) && (  (u%2) )  || // uc odd, lc even
           (u >= 0xC682) && (u <= 0xC685) && ( !(u%2) )  || // uc even, lc odd 
           (u >= 0xC687) && (u <= 0xC688) && (  (u%2) )  || // uc odd, lc even
           (u >= 0xC68B) && (u <= 0xC68C) && (  (u%2) )  || // uc odd, lc even
           (u >= 0xC691) && (u <= 0xC692) && (  (u%2) )  || // uc odd, lc even 
           (u >= 0xC698) && (u <= 0xC699) && ( !(u%2) )  || // uc even, lc odd 
           (u >= 0xC6A0) && (u <= 0xC6A5) && ( !(u%2) )  || // uc even, lc odd 
           (u >= 0xC6A7) && (u <= 0xC6A8) && (  (u%2) )  || // uc odd, lc even 
           (u >= 0xC6AC) && (u <= 0xC6AD) && (  (u%2) )  || // uc odd, lc even 
           (u >= 0xC6AF) && (u <= 0xC6B0) && ( !(u%2) )  || // uc even, lc odd
           (u >= 0xC6B3) && (u <= 0xC6B6) && (  (u%2) )  || // uc odd, lc even 
           (u >= 0xC6B8) && (u <= 0xC6B9) && ( !(u%2) )  || // uc even, lc odd
           (u >= 0xC6BC) && (u <= 0xC6BD) && ( !(u%2) ) ) { // uc even, lc odd
            return(u + 1);
      } /* else {
         return(u); // already uc
      } */
      if (u == 0xC784)
         return(0xC785);
      if (u == 0xC785)
         return(0xC786);
      if (u == 0xC786)
         return(u); // already lc
      if (u == 0xC787)
         return(0xC788);
      if (u == 0xC788)
         return(0xC789);
      if (u == 0xC789)
         return(u); // already lc
      if (u == 0xC78A)
         return(0xC78B);
      if (u == 0xC78B)
         return(0xC78C);
      if (u == 0xC78C)
         return(u); // already lc
      if ( (u >= 0xC78D) && (u <= 0xC79C) && (  (u%2) )  || // uc odd, lc even 
           (u >= 0xC79E) && (u <= 0xC7AF) && ( !(u%2) ) ) { // uc even, lc odd
         return(u + 1);
      }
      if (u == 0xC7B1)
         return(0xC7B2);
      if (u == 0xC7B2)
         return(0xC7B3);
      if (u == 0xC7B3)
         return(u); // already lc
      if ( (u >= 0xC7B4) && (u <= 0xC7B5) && ( !(u%2) ) ||  // uc even, lc odd
           // (u >= 0xC7B8) && (u <= 0xC89F) || // uc +1 && even lc -1 && odd
           (u >= 0xC7B8) && (u <= 0xC89F) && ( !(u%2) ) ||  // uc even, lc odd
           (u >= 0xC8A2) && (u <= 0xC8B3) && ( !(u%2) ) ||  // uc even, lc odd
           (u >= 0xC8BB) && (u <= 0xC8BC) && (  (u%2) ) ||  // uc odd, lc even
           (u >= 0xC981) && (u <= 0xC982) && (  (u%2) ) ||  // uc odd, lc even
           (u >= 0xC986) && (u <= 0xC98F) && ( !(u%2) ) ) { // uc even, lc odd
         return(u + 1);
      }
      // Greek
      if ( (u >= 0xCE91) && (u <= 0xCE9F) ) // between ALPHA and OMICRON
         return(u + 0x20);
      if ( (u >= 0xCEA0) && (u <= 0xCEA1) ) // between PI and RHO
         return(u + 0xE0);
      if (  u == 0xCEA2 ) // unknown character
         return(u); // do nothing
      if ( (u == 0xCEA3) ) // SIGMA has two lc forms: 0xCF82 & 0xCF83
         return(u + 0xDF);
      if ( (u >= 0xCEA4) && (u <= 0xCEAB) ) // between TAU and UPSILON with dialytika
         return(u + 0xE0);
      if ( (u >= 0xCEAC) && (u <= 0xCEBF) ) // betw alpha & omicron + prec accented chars
         return(u); // already lc
      if ( (u >= 0xCF80) && (u <= 0xCF8E) ) // between pi and omega with tonos
         return(u); // already lc
      if( u >= 0xd080 && u <= 0xd08f ) // Serbian, Ukrainian, etc.
         return(u + 0x0110);
      if( u >= 0xd090 && u <= 0xd09f ) // Russian A through P
         return(u + 0x0020);
      if( u >= 0xd0a0 && u <= 0xd0af ) // Russian R through Ya
         return(u + 0x00e0);
      if( u == 0xd380 ) return(0xd38f); // palochka
      // the following ranges require special treatment
      if(
         ( // the following ranges are converted by adding one ...
          ( // omega through koppa OR
            (u >= 0xd1a0 && u <= 0xd280) ||
            // i kratkoe with tail through Abkhasian che with descender
            (u >= 0xd28a && u <= 0xd2be) || // OR
            // a with breve through m with tail
            (u >= 0xd390 && u <= 0xd4ae) 
          )
          && !(u%2) // ... AND IF THE LETTER IS EVEN
         )
         || // OR
         ( // zhe with breve through L with descender
            (u >= 0xd381 && u <= 0xd38d)
            && (u%2) // ... IF THE LETTER IS ODD
          )
        )
         return(u + 0x0001);
      return(u);
   }

   // if u is a three-byte character, use tolower() for now
   if( is3butf8(u) ) {
      if (u == 0xE1BA9E) // UC German scharfes es, eszet
         return(0xC39F); // lc German scharfes es, eszet
      return(tolower(u));
   }
   // if u is a four-byte character, use tolower() for now
   if( is4butf8(u) ) {
      return(tolower(u));
   }
}

/***************************************************************
*                     FUNCTION DEFINITIONS                    *
***************************************************************/
// all functions have been put into separate files

