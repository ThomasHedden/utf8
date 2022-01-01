#include <stdio.h>
#include <stdlib.h>  // needed for exit()
#include <stdbool.h> // needed for is1butf8() etc.

/***************************************************************
fgetu(FILE *)
Pre:            This function takes a FILE* pointer to an open
                file stream of a file that contain UTF-8 strings.
Post:           This function returns an int representing the
                next UTF-8 character in the string. This int
                comprises either 1, 2, 3, or 4 bytes, depending
                on what is being encoded. That is, this int can
                be thought of as 1, 2, 3, or 4 unsigned chars
                concatenated together.
                It is the responsibility of the calling function
                to handle the returned int correctly.
                The FILE pointer is advanced to the point where
                the next UTF8 character should begin.
                This function attempts to recover when it finds
                malformed UTF-8 characters: when an illegal
                byte sequence is encountered, this function
                prints the hex value of the offending byte to
                stderr and discards it, and returns 0xEFBFBD
                (U+FFFD), the replacement character. On the
                next iteration it advances to the next byte.
                For example, the sequence 0xD096 (U+0416)
                represents a letter of the Russian alphabet.
                The sequence 0xD041 is illegal, because 0x41
                is not a valid trailing byte. So, an error
                message is printed saying that 0xD0 has been
                discarded, and on the next iteration the
                function will try to process 0x41.
Functions used: only standard library functions
Includes:       stdio.h; stdlib.h (for exit()); stdbool.h
Used in:        main(); ()                                     */
int fgetu(FILE * opened_file_stream) {
   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(EXIT_FAILURE);
   }
   /***************************************************************
   *                     FUNCTION DECLARATIONS                    *
   ***************************************************************/
   bool  is1butf8(unsigned int); // returns true if 1-byte UTF-8
   bool  isb1of2b(unsigned int); // true if 1st byte of 2-byte UTF-8
   bool  istbutf8(unsigned int); // true if trailing byte of UTF-8
   bool  isb1of3b(unsigned int); // true if 1st byte of 3-byte UTF-8
   bool  isb1of4b(unsigned int); // true if 1st byte of 4-byte UTF-8
   bool  isb1ofu8(unsigned int); // true if 1st byte of any UTF-8 character

   // should the chars be signed chars ??
   unsigned char c1; // first char of a UTF-8 sequence
   unsigned char c2; // second char of a UTF-8 sequence
   unsigned char c3; // second third char of a UTF-8 sequence
   unsigned char c4; // fourth char of a UTF-8 sequence
   unsigned int  b1; // first byte of a UTF-8 sequence
   unsigned int  b2; // second byte of a UTF-8 sequence
   unsigned int  b3; // third byte of a UTF-8 sequence
   unsigned int  b4; // fourth byte of a UTF-8 sequence

   // look at first byte in string
   c1 = fgetc(opened_file_stream);
   if( feof(opened_file_stream) ) {
      return(0);
   }
   // found single-byte character
   if( is1butf8(c1) ) {
      return( (int) c1 );
   // found first byte of a two-byte character
   } else if( isb1of2b(c1) ) {
      // get second byte
      c2 = fgetc(opened_file_stream); 
      // check whether second byte is a trailing byte
      if ( istbutf8(c2) ) { // second byte is a trailing byte
         ; // do nothing: processing occurs after all checks completed
      } else { // second byte is NOT a trailing byte
         fprintf(stderr, "Failed to get second byte of two-byte character.\n");
         fprintf(stderr, "Discarded first byte was 0x%X\n", (unsigned int) c1);
	 // Although second byte is not a valid trailing byte, if it
	 // is a valid first byte, decrement the index to allow this
	 // second byte to be picked up on the loop's next iteration.
	 // This function's return type is int, so return replacement
	 // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
	 // It is the responsibility of the calling function to process
	 // this replacement character, either ignoring it or printing
	 // the question mark. 
	 // If second byte is NOT a valid first character, discard it
	 // as well as first byte.
	 if ( isb1ofu8(c2) ) { // second byte is valid 1st byte so let
	                       // it to be picked on next iteration
            ungetc(c2, opened_file_stream);
            return(0xEFBFBD); // return replacement character instead of
	                      // invalid first byte of 2-byte sequence 
         } else { // second byte is not even a valid first character,
		  // so discard it, too: don't let it be picked up
		  // on next iteration
            fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
            return(0xEFBFBD); // replacement character
         }
      }
      // process valid 2-byte sequence
      b1 = (unsigned int) c1;
      b1 <<= 8; // promote by 8 bits
      return( (int) (b1 + c2) );
   // found first byte of a three-byte character
   } else if( isb1of3b(c1) ) {
      // get second byte
      c2 = fgetc(opened_file_stream); 
      // check whether second byte is a trailing byte
      if ( istbutf8(c2) ) { // second byte is a trailing byte
         ; // do nothing: processing occurs after all checks completed
      } else { // second byte is NOT a trailing byte
         fprintf(stderr, "Failed to get second byte of three-byte character.\n");
         fprintf(stderr, "Discarded first byte was 0x%X\n", (unsigned int) c1);
	 // Although second byte is not a valid trailing byte, if it
	 // is a valid first byte, decrement the index to allow this
	 // second byte to be picked up on the loop's next iteration.
	 // This function's return type is int, so return replacement
	 // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
	 // It is the responsibility of the calling function to process
	 // this replacement character, either ignoring it or printing
	 // the question mark. 
	 // If second byte is NOT a valid first character, discard it
	 // as well as first byte.
	 if ( isb1ofu8(c2) ) { // second byte is valid 1st byte so let
	                       // it to be picked on next iteration
            ungetc(c2, opened_file_stream);
            return(0xEFBFBD); // return replacement character instead of
	                      // invalid first byte of 3-byte sequence 
         } else { // second byte is not even a valid first character,
		  // so discard it, too: don't let it be picked up
		  // on next iteration
            fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
            return(0xEFBFBD); // replacement character
         }
      }
      // get third byte
      c3 = fgetc(opened_file_stream); 
      // check whether third byte is a trailing byte
      if ( istbutf8(c3) ) { // third byte is a trailing byte
         ; // do nothing: processing occurs after all checks completed
      } else { // third byte is NOT a trailing byte
         fprintf(stderr, "Failed to get third byte of three-byte character.\n");
         fprintf(stderr, "Discarded first byte was 0x%X\n", (unsigned int) c1);
         fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
	 // Although third byte is not a valid trailing byte, if it
	 // is a valid first byte, decrement the index to allow this
	 // third byte to be picked up on the loop's next iteration.
	 // This function's return type is int, so return replacement
	 // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
	 // It is the responsibility of the calling function to process
	 // this replacement character, either ignoring it or printing
	 // the question mark. 
	 // If third byte is NOT a valid first character, discard it
	 // as well as first and second bytes.
	 if ( isb1ofu8(c3) ) { // third byte is valid 1st byte so let
	                       // it to be picked on next iteration
            ungetc(c3, opened_file_stream);
            return(0xEFBFBD); // return replacement character instead of
	                      // invalid first and second bytes of 3-byte
	    		      // sequence 
         } else { // third byte is not even a valid first character,
		  // so discard it, too: don't let it be picked up
		  // on next iteration
            fprintf(stderr, "Discarded third byte was 0x%X\n", (unsigned int) c3);
            return(0xEFBFBD); // replacement character
         }
      }
      // process valid 3-byte sequence
      b1 = (unsigned int) c1;
      b1 <<= 16; // promote by 16 bits
      b2 = (unsigned int) c2;
      b2 <<= 8;  // promote by 8 bits
      // if the three-byte character is the UTF-8 byte order mark ...
      if( (b1 + b2 + (unsigned int) c3) == 0x00EFBBBF )
         fprintf(stderr, "Found UTF-8 BOM\n");
      return( (int) (b1 + b2 + c3));
   // found first byte of a four-byte character
   } else if( isb1of4b(c1) ) {
      // get second byte
      c2 = fgetc(opened_file_stream); 
      // check whether second byte is a trailing byte
      if ( istbutf8(c2) ) { // second byte is a trailing byte
         ; // do nothing: processing occurs after all checks completed
      } else { // second byte is NOT a trailing byte
         fprintf(stderr, "Failed to get second byte of four-byte character.\n");
         fprintf(stderr, "Discarded first byte was 0x%X\n", (unsigned int) c1);
	 // Although second byte is not a valid trailing byte, if it
	 // is a valid first byte, decrement the index to allow this
	 // second byte to be picked up on the loop's next iteration.
	 // This function's return type is int, so return replacement
	 // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
	 // It is the responsibility of the calling function to process
	 // this replacement character, either ignoring it or printing
	 // the question mark. 
	 // If second byte is NOT a valid first character, discard it
	 // as well as first byte.
	 if ( isb1ofu8(c2) ) { // second byte is valid 1st byte so let
	                       // it to be picked on next iteration
            ungetc(c2, opened_file_stream);
            return(0xEFBFBD); // return replacement character instead of
	                      // invalid first byte of 4-byte sequence 
         } else { // second byte is not even a valid first character,
		  // so discard it, too: don't let it be picked up
		  // on next iteration
            fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
            return(0xEFBFBD); // replacement character
         }
      }
      // get third byte
      c3 = fgetc(opened_file_stream); 
      // check whether third byte is a trailing byte
      if ( istbutf8(c3) ) { // third byte is a trailing byte
         ; // do nothing: processing occurs after all checks completed
      } else { // third byte is NOT a trailing byte
         fprintf(stderr, "Failed to get third byte of four-byte character.\n");
         fprintf(stderr, "Discarded first byte was 0x%X\n", (unsigned int) c1);
         fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
	 // Although third byte is not a valid trailing byte, if it
	 // is a valid first byte, decrement the index to allow this
	 // third byte to be picked up on the loop's next iteration.
	 // This function's return type is int, so return replacement
	 // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
	 // It is the responsibility of the calling function to process
	 // this replacement character, either ignoring it or printing
	 // the question mark. 
	 // If third byte is NOT a valid first character, discard it
	 // as well as first and second bytes.
	 if ( isb1ofu8(c3) ) { // third byte is valid 1st byte so let
	                       // it to be picked on next iteration
            ungetc(c3, opened_file_stream);
            return(0xEFBFBD); // return replacement character instead of
	                      // invalid first and second bytes of 4-byte
			      // sequence 
         } else { // third byte is not even a valid first character,
		  // so discard it, too: don't let it be picked up
		  // on next iteration
            fprintf(stderr, "Discarded third byte was 0x%X\n", (unsigned int) c3);
            return(0xEFBFBD); // replacement character
         }
      }
      // get fourth byte
      c4 = fgetc(opened_file_stream); 
      // check whether fourth byte is a trailing byte
      if ( istbutf8(c4) ) { // fourth byte is a trailing byte
         ; // do nothing: processing occurs after all checks completed
      } else { // fourth byte is NOT a trailing byte
         fprintf(stderr, "Failed to get fourth byte of four-byte character.\n");
         fprintf(stderr, "Discarded first byte was 0x%X\n", (unsigned int) c1);
         fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
         fprintf(stderr, "Discarded third byte was 0x%X\n", (unsigned int) c3);
	 // Although fourth byte is not a valid trailing byte, if it
	 // is a valid first byte, decrement the index to allow this
	 // fourth byte to be picked up on the loop's next iteration.
	 // This function's return type is int, so return replacement
	 // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
	 // It is the responsibility of the calling function to process
	 // this replacement character, either ignoring it or printing
	 // the question mark. 
	 // If fourth byte is NOT a valid first character, discard it
	 // as well first, second, and third bytes.
	 if ( isb1ofu8(c4) ) { // fourth byte is valid 1st byte so let
	                       // it to be picked on next iteration
            ungetc(c4, opened_file_stream);
            return(0xEFBFBD); // return replacement character instead of
	                      // invalid first, second, and third bytes
			      // of 4-byte sequence 
         } else { // fourth byte is not even a valid first character,
		  // so discard it, too: don't let it be picked up
		  // on next iteration
            fprintf(stderr, "Discarded fourth byte was 0x%X\n", (unsigned int) c4);
            return(0xEFBFBD); // replacement character
         }
      }
      // process valid 4-byte sequence
      b1 = (unsigned int) c1;
      b1 <<= 24; // promote by 24 bits
      b2 = (unsigned int) c2;
      b2 <<= 16; // promote by 16 bits
      b3 = (unsigned int) c3;
      b3 <<= 8; // promote by 8 bits
      return( (int) (b1 + b2 + b3 + c4));
   // found illegal sequence
   } else if( ((unsigned int) c1 >= 0xC0) && 
              ((unsigned int) c1 <= 0xC1) ) {
      fprintf(stderr, "Discarded illegal byte: 0x%X\n", (unsigned int) c1);
      return(0xEFBFBD);
   // found illegal sequence that might be a BOM for UTF-16 or UTF-32 encoding
   } else if( ((unsigned int) c1 >= 0xF5) && 
              ((unsigned int) c1 <= 0xFF) ) {
      c2 = fgetc(opened_file_stream);
      b1 = (unsigned int) c1;
      b1 <<= 8; // promote by 8 bits
      // b1 + c2 is UTF-16 big-endian byte order mark
      // or second half of UTF-32 big-endian BOM
      if( (b1 + (unsigned int) c2) == 0xFEFF ) {
         fprintf(stderr, "Found UTF-16 big-endian BOM or ");
         fprintf(stderr, "2nd half of UTF-32 big-endian BOM\n");
         return(0xEFBFBD); // replacement character
      }
      // b1 + c2 is UTF-16 little-endian byte order mark
      // or second half of UTF-32 little-endian BOM
      if( (b1 + (unsigned int) c2) == 0xFFFE ) {
         fprintf(stderr, "Found UTF-16 little-endian BOM or ");
         fprintf(stderr, "2nd half of UTF-32 little-endian BOM\n");
         return(0xEFBFBD); // replacement character
      }
      fprintf(stderr, "Discarded illegal byte: 0x%X\n", (unsigned int) c1);
      // Although second byte is not a valid second byte of a BOM,
      // if it is a valid first byte, decrement the index to allow
      // this second byte to be picked up on the loop's next iteration.
      // This function's return type is int, so return replacement
      // character 0xEFBFBD (U+FFFD) to satisfy this requirement.
      // It is the responsibility of the calling function to process
      // this replacement character, either ignoring it or printing
      // the question mark. 
      // If second byte is NOT a valid first character, discard it
      // as well as first byte.
      if ( isb1ofu8(c2) ) { // second byte is valid 1st byte so let
                            // it to be picked on next iteration
         ungetc(c2, opened_file_stream);
         return(0xEFBFBD);  // return replacement character instead of
                            // invalid first byte of 2-byte sequence 
      } else { // second byte is not even a valid first character,
               // so discard it, too: don't let it be picked up
               // on next iteration
         fprintf(stderr, "Discarded second byte was 0x%X\n", (unsigned int) c2);
         return(0xEFBFBD); // replacement character
      }
   } else if( istbutf8(c1) ) { // found out-of-place trailing byte
      fprintf(stderr, "Found out-of-place trailing byte: 0x%X\n", (unsigned int) c1);
      return(0xEFBFBD); // replacement character
   } else { // found unknown byte
      fprintf(stderr, "An unknown error occurred: %0xX\n", (unsigned int) c1);
      return(0xEFBFBD);
   }
   // close file stream
   fclose(opened_file_stream);
}; /* end fgetu(FILE *) */

