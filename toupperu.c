#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <stdbool.h> // needed for is1butf8(), etc.
#include <ctype.h>   // needed for toupper()

/***************************************************************
* unsigned int toupperu(unsigned int)
* This function takes as its argument an unsigned four-byte
* int representing a UTF-8 character, and attempts to return
* the upper-case equivalent.
* This function should work for the UTF-8 characters
* in Latin A and B, Greek, and Cyrillic. Otherwise,
* it returns the same value as the ctype.h toupper()
* function.
* Please note that if lowercase German scharfes es
* (eszet, 0xC39F) is passed to this function it
* returns uppercase German scharfes es (0xE1BA9E).
* If it is desired to get two uppercase letters S
* the calling function can do this by replacing
* uppercase German scharfes es (0xE1BA9E) by "SS"
* Pre:            an unsigned int representing a UTF-8 character
* Post:           an unsigned int representing the corresponding
*                 uppercase UTF-8 character
* Functions used: standard library functions and is1butf8(), etc.
* Includes:       stdio.h, stdlib.h, stdbool.h
* Used in:                                                 */

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

unsigned int toupperu(unsigned int u) {
   // this program requires that an int have 4 bytes
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // if u is a single-byte character, use toupper()
   if( is1butf8(u) )
      return(toupper(u));
   // if u is a two-byte character, need to look at details
   if( is2butf8(u) ) {
      // Latin extended A
      if ( (u >= 0xC380 && u <= 0xC396) ||  // A grave to O dieresis
           (u >= 0xC398 && u <= 0xC39E) )   // O stroke to capital thorn
         return(u); // already is uppercase
      if (  u == 0xC39F )  // lc German scharfes es, eszet
         return(0xE1BA9E); // uc German scharfes es, eszet
      if ( (u >= 0xC3A0 && u <= 0xC3B6) ||  // a grave to o dieresis
           (u >= 0xC3B8 && u <= 0xC3BE) )   // o stroke to lc letter thorn
         return(u - 0x20);
      if (u == 0xC3BF )    // lc Latin y with dieresis
         return(0xC5B8);   // uc Latin y with dieresis
      // Latin extended A and B
      // if ( (u >= 0xC480) && (u <= 0xC7BF) ||
      if ( (u >= 0xC480) && (u <= 0xC4B7) && (  (u%2) )  || // uc even, lc odd 
           (u >= 0xC4B9) && (u <= 0xC588) && ( !(u%2) )  || // uc odd, lc even
           (u >= 0xC58A) && (u <= 0xC5B7) && (  (u%2) )  || // uc even, lc odd 
           (u >= 0xC5B9) && (u <= 0xC5BE) && ( !(u%2) )  || // uc odd, lc even
           (u >= 0xC682) && (u <= 0xC685) && (  (u%2) )  || // uc even, lc odd 
           (u >= 0xC687) && (u <= 0xC688) && ( !(u%2) )  || // uc odd, lc even
           (u >= 0xC68B) && (u <= 0xC68C) && ( !(u%2) )  || // uc odd, lc even
           (u >= 0xC691) && (u <= 0xC692) && ( !(u%2) )  || // uc odd, lc even 
           (u >= 0xC698) && (u <= 0xC699) && (  (u%2) )  || // uc even, lc odd 
           (u >= 0xC6A0) && (u <= 0xC6A5) && (  (u%2) )  || // uc even, lc odd 
           (u >= 0xC6A7) && (u <= 0xC6A8) && ( !(u%2) )  || // uc odd, lc even 
           (u >= 0xC6AC) && (u <= 0xC6AD) && ( !(u%2) )  || // uc odd, lc even 
           (u >= 0xC6AF) && (u <= 0xC6B0) && (  (u%2) )  || // uc even, lc odd
           (u >= 0xC6B3) && (u <= 0xC6B6) && ( !(u%2) )  || // uc odd, lc even 
           (u >= 0xC6B8) && (u <= 0xC6B9) && (  (u%2) )  || // uc even, lc odd
           (u >= 0xC6BC) && (u <= 0xC6BD) && (  (u%2) ) ) { // uc even, lc odd
            return(u - 1);
      }
      if (u == 0xC784)
         return(u); // already uc
      if (u == 0xC785)
         return(0xC784);
      if (u == 0xC786)
         return(0xC785);
      if (u == 0xC787)
         return(u); // already uc
      if (u == 0xC788)
         return(0xC787);
      if (u == 0xC789)
         return(0xC788);
      if (u == 0xC78A)
         return(u); // already uc
      if (u == 0xC78B)
         return(0xC78A);
      if (u == 0xC78C)
         return(0xC78B);
      if ( (u >= 0xC78D) && (u <= 0xC79C) && ( !(u%2) )  || // uc odd, lc even 
           (u >= 0xC79E) && (u <= 0xC7AF) && (  (u%2) ) ) { // uc even, lc odd
         return(u - 1);
      }
      if (u == 0xC7B1)
         return(u); // already uc
      if (u == 0xC7B2)
         return(0xC7B1);
      if (u == 0xC7B3)
         return(0xC7B2);
      if ( (u >= 0xC7B4) && (u <= 0xC7B5) && (  (u%2) ) ||  // uc even, lc odd
           (u >= 0xC7B8) && (u <= 0xC89F) && (  (u%2) ) ||  // uc even, lc odd
           (u >= 0xC8A2) && (u <= 0xC8B3) && (  (u%2) ) ||  // uc even, lc odd
           (u >= 0xC8BB) && (u <= 0xC8BC) && ( !(u%2) ) ||  // uc odd, lc even
           (u >= 0xC981) && (u <= 0xC982) && ( !(u%2) ) ||  // uc odd, lc even
           (u >= 0xC986) && (u <= 0xC98F) && (  (u%2) ) ) { // uc even, lc odd
         return(u - 1);
      }
      // Greek
      // HETA, SAMPI, DIGAMMA
      if ( (u >= 0xCDB0) && (u <= 0xCDB7) && (  (u%2) ) )   // uc even, lc odd
         return(u - 0x01);    // 
// cdb8 cdb9 cdba return u
      if ( (u == 0xCDBB) ) // reversed lunate sigma symbol
         return(0xCFBD);   // reversed lunate SIGMA symbol
      if ( (u == 0xCDBC) ) // dotted lunate sigma symbol
         return(0xCFBE);   // dotted lunate SIGMA symbol
      if ( (u == 0xCDBD) ) // reversed dotted lunate sigma symbol
         return(0xCFBF);   // reversed dotted lunate SIGMA symbol
      if ( (u >= 0xCDBE) && (u <= 0xCEAB) ) // between ALPHA and OMICRON
                                            // plus prec. accented chars
         return(u); // already UC
      if ( (u == 0xCEAC) ) // alpha w tonos
         return(0xCE86);   // ALPHA w tonos
      if ( (u >= 0xCEAD) && (u <= 0xCEAF) ) // betw epsilon w tonos & iota with tonos
         return(u - 0x25);
// what to do with this?
// U+03B0	ΰ	ce b0	GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND TONOS
      if ( (u >= 0xCEB1) && (u <= 0xCEBF) ) // betw alpha & omicron
         return(u - 0x20);
      if ( (u >= 0xCF80) && (u <= 0xCF81) ) // betw pi & rho
         return(u - 0xE0);
      // small letter final sigma or small sigma
      if ( (u == 0xCF82) || (u == 0xCF83) )
         return(0xCEA3);   // SIGMA
      // between tau and omega
      if ( (u >= 0xCF84) && (u <= 0xCF89) )
         return(u - 0xE0);
// what to do with this?
// U+03CA	ϊ	cf 8a	GREEK SMALL LETTER IOTA WITH DIALYTIKA
      if (u == 0xCF8B)   // upsilon with dialytika
         return(0xCEAB); // UPSILON with dialytika
      if ( (u == 0xCF8C) ) // omicron w tonos
         return(0xCE8C);   // OMICRON w tonos
      if ( (u == 0xCF8D) ) // upsilon w tonos
         return(0xCE8E);   // UPSILON w tonos
      if ( (u == 0xCF8E) ) // omega w tonos
         return(0xCE8F);   // OMEGA w tonos
      if ( (u == 0xCF8F) ) // capital KAI symbol
         return(u);        // already UC
// what to do with this?
// U+03D0	ϐ	cf 90	GREEK BETA SYMBOL
      if ( (u == 0xCF91) ) // theta symbol
         return(0xCFB4);   // capital THETA symbol
// what to do with these?
// U+03D2	ϒ	cf 92	GREEK UPSILON WITH HOOK SYMBOL
// U+03D3	ϓ	cf 93	GREEK UPSILON WITH ACUTE AND HOOK SYMBOL
// U+03D4	ϔ	cf 94	GREEK UPSILON WITH DIAERESIS AND HOOK SYMBOL
// U+03D5	ϕ	cf 95	GREEK PHI SYMBOL
// U+03D6	ϖ	cf 96	GREEK PI SYMBOL
      if ( (u == 0xCF97) ) // kai symbol
         return(0xCF8F);   // capital KAI symbol
      // between archaic koppa & Coptic small dei 
      if ( (u >= 0xCF98) && (u <= 0xCFAF) && (  (u%2) ) ) // UC even, lc odd
         return(u - 0x01);
// what to do with these?
// U+03F0	ϰ	cf b0	GREEK KAPPA SYMBOL
// U+03F1	ϱ	cf b1	GREEK RHO SYMBOL
      if ( (u == 0xCFB2) ) // lc lunate sigma symbol
         return(0xCDB9);   // UC lunate SIGMA symbol
      if ( (u == 0xCFB3) ) // lc yot
         return(0xCDBF);   // UC YOT
      if ( (u >= 0xCFB4) && (u <= 0xCFB7) ) // UC THETA symbol to SHO
         return(u);        // already UC
// what to do with these?
// U+03F5	ϵ	cf b5	GREEK LUNATE EPSILON SYMBOL
// U+03F6	϶	cf b6	GREEK REVERSED LUNATE EPSILON SYMBOL
      if ( (u == 0xCFB8) ) // lc sho
         return(0xCFB7);   // UC SHO
      if ( (u >= 0xCFB9) && (u <= 0xCFBA) ) // UC lunate SIGMA symb & SAN
         return(u);        // already UC
      if ( (u == 0xCFBB) ) // lc san
         return(0xCFBA);   // UC SAN
// what to do with this?
// U+03FC	ϼ	cf bc	GREEK RHO WITH STROKE SYMBOL
// cfbd through cfbf already uc
// return(u);

/* problem characters
c4b8
c589
c5b8
c6bf
c680
c681
C686
c689
c68a
c68d
c68e
c68f
c690
c693
c694
c695
c696
c697
C69a
c69b
c69c
c69d
c69e
C6a6
c6a9
c6aa
c6ab
c6ae
c6b1
c6b2
c6b7
c6BA
c6bb
c7b0
c79d
c7b0 
c7b6 
c7b7 
C8A0
C8A1
C8B4 through C8BA
C8BD through C8BF
C983 through C985
C990 through C9BF through CD8F
*/
      // if( u >= 0xd080 && u <= 0xd08f ) // Serbian, Ukrainian, etc.
      if( u >= 0xd190 && u <= 0xd19f ) // Serbian, Ukrainian, etc.
         return(u - 0x0110);
      // The difference between the UC and lc letters in the first
      // half of the Russian alphabet (A-P) is not the same as it
      // is in the second half  (R-Ya)
      // if( u >= 0xd090 && u <= 0xd09f ) // Russian A through P
      if( u >= 0xd0b0 && u <= 0xd0bf ) // Russian a through p
         return(u - 0x0020);
      // if( u >= 0xd0a0 && u <= 0xd0af ) // Russian R through Ya
      if( u >= 0xd180 && u <= 0xd18f ) // Russian r through ya
         return(u - 0x00e0);
      if( u == 0xd38f )
         return(0xd380); // palochka
      // the following ranges require special treatment
      if(
         ( // the following ranges are converted by adding one ...
          ( // omega through koppa OR
            (u >= 0xd1a0 && u <= 0xd281) ||
            // i kratkoe with tail through Abkhasian che with descender
            (u >= 0xd28a && u <= 0xd2bf) || // OR
            // a with breve through m with tail
            (u >= 0xd390 && u <= 0xd4af) 
          )
          && (u%2) // ... AND IF THE LETTER IS ODD
         )
         || // OR
         ( // zhe with breve through L with descender
            (u >= 0xd381 && u <= 0xd38e)
            && !(u%2) // ... AND IF THE LETTER IS EVEN
          )
        )
         return(u - 0x0001);
      return(u); // if cannot match, then keep as is
   }
   // if u is a three-byte character, simply return it
   if( is3butf8(u) )
      return(u);
   // if u is a four-byte character, simply return it
   if( is4butf8(u) )
      return(u);
} // end of toupperu()

/***************************************************************
*                     FUNCTION DEFINITIONS                    *
***************************************************************/
// all functions have been put into separate files

