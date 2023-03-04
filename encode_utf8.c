#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <stdbool.h> // needed for is1butf8() etc.

/***************************************************************/
/*                           README                            */
/***************************************************************/
/* This function takes a Unicode code point in the form
   U+(hex value) and converts it into the hexadecimal value of
   the corresponding UTF-8 character. For example, Unicode code
   point U+0416 is converted to 0xD096, which corresponds to
   Russian capital letter Ж (ZH).
   Written by Thomas Hedden March 2021. Revised January 2022.
*/

/***************************************************************/
/*                 THINGS THAT NEED ATTENTION                  */
/***************************************************************/
/* #include's should be bracketed by INFDEFINE's
   testing statements should be removed
*/

/***************************************************************/
/*               STRUCT DECLARATIONS AND TYPEDEFS              */
/***************************************************************/
typedef unsigned int UNT;
typedef unsigned char UNC;

/***************************************************************
*                     FUNCTION DECLARATIONS                    *
***************************************************************/
bool is1b_Ucp(unsigned int); // Unicode code point to be encoded
                             // with 1 byte
bool is2b_Ucp(unsigned int); // with 2 bytes
bool is3b_Ucp(unsigned int); // with 3 bytes
bool is4b_Ucp(unsigned int); // with 4 bytes

UNT encode_utf8(unsigned int ucp) {
   // variable declarations
   UNT u;   // holds returned hex value of UTF-8 character
   UNC b1;  // holds byte 1 (leftmost, highest order)) of ucp
   UNC b2;  // holds byte 2 (2nd from left, 2nd highest)) of ucp
   UNC b3;  // holds byte 3 (3rd from left, 3rd highest)) of ucp
   UNC b4;  // holds byte 4 (rightmost, lowest order)) of ucp

   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) {
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   // found Unicode code point to be encoded with 1 byte
   if( is1b_Ucp(ucp) ) {
   // if( (ucp >= 0x00000000) && (ucp <= 0x0000007F) ) {
      // procedure for encoding 1-byte UTF-8 character:
      // simply cast ucp as unsigned char
      b1 = (UNC) (ucp & 0x0000007F);
      u  = (UNT) b1;
      // fprintf(stdout, "code point is U+%04x, u is 0x%x, UTF-8 character is %c\n", ucp, u, b1);
   }
   // found Unicode code point to be encoded with 2 bytes
   if( is2b_Ucp(ucp) ) {
   // if( (ucp >= 0x00000080) && (ucp <= 0x000007FF) ) {
      /* procedure for encoding 2-byte UTF-8 charcter:
      procedure for high order byte b1 of 2-byte utf-8 character:
      need last 3 bits of high-order byte of code point ucp
      first, shift high-order byte 8 bits to the right u >> 8,
      bitwise and with 0x07 ( (ucp >> 8) & 0x07), 
      shift result 2 bits to the left ( (ucp >> 8) & 0x07) << 2, 
      then, mask low-order byte of ucp with 0xC0 (ucp & 0x000000C0),
      shift six bits to the right (ucp & 0x000000C0) >> 6
      add these two bytes (((ucp >> 8) & 0x07) << 2 ) + ((ucp & 0xC0) >> 6)
      add sum to 0xC0
      ((((ucp >> 8) & 0x07) << 2 ) + ((ucp & 0xC0) >> 6) + 0xC0),
      bitwise shift sum 8 bits to the left
      ((((ucp >> 8) & 0x07) << 2 ) + ((ucp & 0xC0) >> 6) + 0xC0) << 8
      procedure for low order byte b2 of 2-byte utf-8 character:
      mask low order byte of original ucp with 0x003F (ucp & 0x0000003F)
      add that to 0x80 ((ucp & 0x0000003F) + 0x80)
      add these:
      ((((ucp >> 8) & 0x07) << 2 ) + ((ucp & 0xC0) >> 6) + 0xC0) << 8
      PLUS
      ((ucp & 0x0000003F) + 0x80) */
      b1 = (UNC) (((((ucp >> 8) & 0x07) << 2 ) + ((ucp & 0x000000C0) >> 6) + 0xC0));
      b2 = (UNC) ((ucp & 0x0000003F) + 0x80);
      u = (UNT) ((b1 << 8) + b2);
   }
   // found Unicode code point to be encoded with 3 bytes
   if( is3b_Ucp(ucp) ) {
   // if( (ucp >= 0x00000800) && (ucp <= 0x0000FFFF) ) {
      /* procedure for encoding 3-byte UTF-8 charcter:
      procedure for high order byte b1 of 3-byte utf-8 character:
      need first 4 (high-order) bits of high-order byte of code point ucp
      first, shift high-order byte 8 bits to the right ucp >> 8,
      bitwise and with 0xF0 ( (ucp >> 8) & 0xF0), 
      shift result 4 bits to the right ( (ucp >> 8) & 0xF0) >> 4, 
      add result to 0xE0 (((ucp >> 8) & 0xF0) >> 4) + 0xE0
      the result is the first (high-order) byte of this 3-byte UTF-8 character
      shift it 16 bits to the left ((((u >> 8) & 0xF0) >> 4) + 0xE0) << 16
      procedure for second (middle) byte b2 of 3-byte utf-8 character:
      take original high-order byte, shift it 8 bits to the right ucp >> 8,
      mask it with 0x0F ( (ucp >> 8) & 0x0F )
      shift it 2 bits to the left ( (ucp >> 8) & 0x0F ) << 2
      then, mask low-order byte of ucp with 0xC0 (ucp & 0x000000C0),
      shift result six bits to the right (ucp & 0x000000C0) >> 6
      add these two bytes 
      (((ucp >> 8) & 0x0F) << 2 ) + ((ucp & 0xC0) >> 6)
      add sum to 0x80 ((((ucp >> 8) & 0x0F) << 2 ) + ((ucp & 0xC0) >> 6) + 0x80)
      this is the middle byte of the 3-byte UTF-8 character
      shift this 8 bits to the left:
      ((((ucp >> 8) & 0x0F) << 2 ) + ((ucp & 0xC0) >> 6) + 0x80) < 8
      procedure for third (low-order) byte of 3-byte UTF-8 character:
      mask low order byte of original ucp with 0x003F (u & 0x0000003F)
      add that to 0x80 ((ucp & 0x0000003F) + 0x80)
      add these:
      ((((ucp >> 8) & 0xF0) >> 4) + 0xE0) << 16
      PLUS
      ((((ucp >> 8) & 0x0F) << 2 ) + ((ucp & 0xC0) >> 6) + 0x80) < 8
      PLUS
      ((ucp & 0x0000003F) + 0x80) */
      b1 = (UNC) (((((ucp >> 8) & 0xF0) >> 4) + 0xE0));
      b2 = (UNC) (((((ucp >> 8) & 0x0F) << 2 ) + ((ucp & 0xC0) >> 6) + 0x80));
      b3 = (UNC) ((ucp & 0x0000003F) + 0x80);
      u = (UNT) ( (b1 << 16) + (b2 << 8) + b3 );
   }
   // found Unicode code point to be encoded with 4 bytes
   if( is4b_Ucp(ucp) ) {
   // if( (ucp >= 0x00010000) && (ucp <= 0x0010FFFF) ) {
      /* procedure for encoding 4-byte UTF-8 charcter:
      procedure for high order byte b1 of 4-byte utf-8 character:
      demote high-order byte by 16 bits ucp >> 16
      get middle 3 bits of byte 1 (ucp >> 16) & 0x1C
      shift result 2 bits to the right ((ucp >> 16) & 0x1C) >> 2
      add 0xF0 (((ucp >> 16) & 0x1C) >> 2) + 0xF0
      this is the high-order byte of the 4-byte UTF-8 character
      this has to be promoted by 24 bits
      ((((ucp >> 16) & 0x1C) >> 2) + 0xF0) << 24
      procedure for 2nd (2nd highest order) byte b2 of 4-byte utf-8 character:
      demote high-order byte by 16 bits ucp >> 16
      get 2 lowest bits of this byte (ucp >> 16) & 0x03
      shift these 2 bits 4 bits to the left ((ucp >> 16) & 0x03) << 4
      now, demome to0 two bytes by 8 bits to the right ucp >> 8
      mask out highest order byte and 4 lower bits of 2nd order byte:
      (ucp >> 8) & 0x00f0
      this leaves the highest 4 bits of the middle byte of byte 2
      shift these 4 bits to the right by 4 bits
      ((ucp >> 8) & 0x00f0) >> 4
      add the lowest 2 bits of the high-order byte and the highest
      4 bits of the second byte, and also add 0x80
      (((ucp >> 16) & 0x03) << 4) + ((ucp >> 8) & 0x00f0) >> 4 + 0x80
      this sum is the second byte (second highest-order) byte
      promote this byte by 16 bits
      ((((ucp >> 16) & 0x03) << 4) + ((ucp >> 8) & 0x00f0) >> 4 + 0x80) << 16
      procedure for third (third highest order) byte b3 of 4-byte utf-8 character:
      demote 2nd byte (2nd highest-order byte) by 8 bits ucp >> 8
      mask out 4 top bits to get 4 lowest-order bits in second byte
      (ucp >> 8) & 0x000F
      shift these 4 bits by 2 bits to the left ((ucp >> 8) & 0x000F) << 2
      now get the 2 highest-order bits of the third byte ucp & 0x0000C0
      shift these 2 bits 6 bits to the right (ucp & 0x0000c0) >> 6
      add this to the 4 lowest bits of byte two and add 0x80
      (((ucp >> 8) & 0x000F) << 2) + ((ucp & 0x0000c0) >> 6) + 0x80
      This sum is the third byte (third highest-order) byte
      promote this byte 8 bits to the left
      ((((ucp >> 8) & 0x000F) << 2) + ((ucp & 0x0000c0) >> 6) + 0x80) << 8
      procedure for fourth (lowest-order) byte:
      mask low order byte of original ucp with 0x003F (u & 0x0000003F)
      ucp & 0x00003F
      add 0x80 (ucp & 0x00003F) + 0x80
      to get the overall hex value, add these:
      ((((ucp >> 16) & 0x1C) >> 2) + 0xF0) << 24
      PLUS
      ((((ucp >> 16) & 0x03) << 4) + ((ucp >> 8) & 0x00f0) >> 4 + 0x80) << 16
      PLUS
      ((((ucp >> 8) & 0x000F) << 2) + ((ucp & 0x0000c0) >> 6) + 0x80) << 8
      PLUS
      add 0x80 (ucp & 0x00003F) + 0x80
      */
      b1 = (UNC) ((((ucp >> 16) & 0x1C) >> 2) + 0xF0);
      b2 = (UNC) ((((ucp >> 16) & 0x03) << 4) + ((((ucp >> 8) & 0x00f0)) >> 4) + 0x80);
      b3 = (UNC) ((((ucp >> 8) & 0x000F) << 2) + ((ucp & 0x0000c0) >> 6) + 0x80);
      b4 = (UNC) ((ucp & 0x0000003F) + 0x80);
      u  = (UNT) ( (b1 << 24) + (b2 << 16) + (b3 << 8) + b4 );
   }
   return(u);
}


