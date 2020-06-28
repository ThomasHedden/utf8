#include <stdio.h>
#include <stdlib.h> // needed for exit

/***************************************************************
strlenu8(char *)
Pre:            This function takes a pointer to a UTF-8 string.
Post:           This function returns an int giving the number
                of UTF8 characters in the string. The characters
                may be 8-bit bytes or characters of two bytes
                or three bytes or four bytes. 
Functions used: only standard library functions
Includes:       stdio.h and stdlib.h (for exit())
Used in:        main()                                         */
size_t strlenu8(char * input_string) {
   unsigned char c1; // first char of a UTF-8 sequence
   unsigned char c2; // second char of a UTF-8 sequence
   unsigned char c3; // third char of a UTF-8 sequence
   unsigned char c4; // fourth char of a UTF-8 sequence
   unsigned short int string_length = 0;

   // this program requires that the size of an int be 4 bytes
   if( sizeof(int) != 4 ) { 
      fprintf(stderr, "sizeof(int) is not 4!\n");
      exit(44);
   }

   // loop through all characters in string
   for(int i = 0; input_string[i] != '\0'; i++) {
      c1 = input_string[i];
      // found single-byte character
      if( (unsigned int) c1 >= 0x00000000 && (unsigned int) c1 <= 0x0000007F ) {
         // single-byte character found is a new line or a carriage return
         if( (unsigned int) c1 == 0x0000000A || (unsigned int) c1 == 0x0000000D )
            continue; // do nothing: don't count
         string_length++;
	 continue;
      // found first byte of a two-byte character
      } else if( (unsigned int) c1 >= 0x000000C2 && (unsigned int) c1 <= 0x000000DF ) {
	 // get second byte
         c2 = input_string[++i]; 
	 // check whether second byte is the trailing byte of a multi-byte character
         if( (unsigned int) c2 >= 0x00000080 && (unsigned int) c2 <= 0x000000BF ) {
            ;
	 } else {
            fprintf(stderr, "failed to get second byte of two-byte character: %c\n", c1);
	    exit(2);
         }
         string_length++;
	 continue;
      // found first byte of a three-byte character
      } else if( (unsigned int) c1 >= 0x000000E0 &&  (unsigned int) c1 <= 0x000000EF ) {
	 // get second byte
         c2 = input_string[++i]; 
	 // check whether second byte is the trailing byte of a multi-byte character
         if( (unsigned int) c2 >= 0x00000080 &&  (unsigned int) c2 <= 0x000000BF ) {
            ;
	 } else {
            fprintf(stderr, "failed to get second byte of three-byte character: %c\n", c1);
	    exit(3);
         }
	 // get third byte
         c3 = input_string[++i]; 
	 // check whether third byte is the trailing byte of a multi-byte character
         if( (unsigned int) c3 >= 0x00000080 &&  (unsigned int) c3 <= 0x000000BF ) {
            ;
	 } else {
            fprintf(stderr, "failed to get third byte of three-byte character: %c\n", c1);
	    exit(3);
         }
         string_length++;
	 continue;
      // found first byte of a four-byte character
      } else if( (unsigned int) c1 >= 0x000000F0 &&  (unsigned int) c1 <= 0x000000F4 ) {
	 // get second byte
         c2 = input_string[++i]; 
	 // check whether second byte is the trailing byte of a multi-byte character
         if( c2 >= 0x00000080 &&  (unsigned int) c2 <= 0x000000BF ) {
            ;
	 } else {
            fprintf(stderr, "failed to get second byte of four-byte character: %c\n", c1);
	    exit(4);
         }
	 // get third byte
         c3 = input_string[++i]; 
	 // check whether third byte is the trailing byte of a multi-byte character
         if( c3 >= (unsigned int) 0x00000080 &&  (unsigned int) c3 <= 0x000000BF ) {
            ;
	 } else {
            fprintf(stderr, "failed to get third byte of four-byte character: %c\n", c1);
	    exit(4);
         }
	 // get fourth byte
         c4 = input_string[++i]; 
	 // check whether fourth byte is trailing byte of a multi-byte character
         if( (unsigned int) c4 >= 0x00000080 &&  (unsigned int) c4 <= 0x000000BF ) {
            ;
	 } else {
            fprintf(stderr, "failed to get fourth byte of four-byte character: %c\n", c1);
	    exit(4);
         }
         string_length++;
	 continue;
      // illegal sequence
      } else if( ((unsigned int) c1 >= 0x000000C0 && (unsigned int) c1 <= 0x000000C1 ) ||
                 ((unsigned int) c1 >= 0x000000F5 && (unsigned int) c1 <= 0x000000FF ) ) {
         fprintf(stderr, "illegal byte: %c\n", c1);
	 continue;
      } else {
         fprintf(stderr, "An unknown error occurred: %c\n", c1);
	 continue;
      }
   }
   return((size_t) string_length);
}; /* end strlenu8() */

