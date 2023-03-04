UTF-8 Tutorial

Every UTF-8 character is represented by one to four unsigned chars. The variable length of UTF-8 characters presents complexity when processing them. For example, getting the next UTF-8 character in a string or a stream using a for() loop cannot be done simply by advancing the index by a fixed number of bytes. However, the way that UTF-8 was designed makes it possible to determine how many bytes the index should advance. The first byte of a UTF-8 character shows how many characters the entire UTF-8 character contains:

This is easy to understand by viewing the characters in binary format:
A single-byte UTF-8 character must have the format 0xxxxxxx.
A two-byte UTF-8 character must have the format    110xxxxx 10xxxxxx.
A three-byte UTF-8 character have the format       1110xxxx 10xxxxxx 10xxxxxx.
A four-byte UTF-8 character must have the format   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx.
Thus, the UTF-8 encoding is a Huffman encoding. There can never be any ambiguity about what a sequence of bytes stands for.

When writing programs to process UTF-8 text, it is easier to use the ranges of the respective bytes:
A single-byte UTF-8 character must fall in the range 0x0000 to 0x007F;
The first byte of a two-byte UTF-8 character must fall in the range 0x00C2 to 0x00DF;
The first byte of a three-byte UTF-8 character must fall in the range 0x00E0 to 0x00EF;
The first byte of a four-byte UTF-8 character must fall in the range 0x00F0 to 0x00F4;
Any trailing byte (second, third, or fourth byte) of a multi-byte UTF-8 character must fall in the range 0x0080 to 0x00BF.
The byte ranges 0xC0 to 0xC1 and 0xF5 to 0xFF are illegal.

For simplicity, I have created simple boolean functions that allow testing of bytes without needing to use the specific values of the ranges:
is1butf8.c
isb1of2b.c
isb1of3b.c
isb1of4b.c 
istbutf8.c 
This makes it easy to test a byte and assign it to one of the five types of bytes listed above, and the unambiguous assignment of the different kinds of bytes in turn makes it possible to determine from a byte how to handle the byte(s) that follow it. In a loop, for example a for() loop, the problem of advancing the index by the right number of bytes remains, because byte testing is done within an iteration of the loop, but the advancement of the index is usually done at the end of an iteration. The solution to this is to pass a pointer to the index into the loop so that the index can also be advanced inside a loop iteration as well as at the end of an iteration. The following code block illustrates how to do this. (The functions getu() and putu() are explained elsewhere.)
```
unsigned int u;
for(int i = 0; (u = getu(my_string, &i)) != '\0'; i++) {
   putu(u);
   fprintf(stdout, " ");
}
fprintf(stdout, "\n");
```
The important thing to understand about UTF-8 processing is to represent every "character" (not char!) as an unsigned int. The int must be a four-byte int, that is it must comprise 32 bits. A short int will not work, nor will a long int, nor will a signed int, nor will a char. Every character requires an unsigned, four-byte int during processing. Depending on its value, this character may be OUTPUT in char or a short int, but during processing it must be represented as an unsigned four-byte int. A program processing UTF-8 characters should check that the platform on which it is running uses a four-byte int, and exit if this is not the case.

Functions must return a signed 4-byte int

That being said, remember that functions that access text often use return values such as EOF, which is a negative int, usually -1. For this reason, functions that process UTF-8 text and return a UTF-8 character should have a return value of a signed four-byte int rather than an unsigned four-byte int. Otherwise, it will not be possible to recognize when the function reaches an end condition such as an EOF. After the return value is obtained and processed, the return value can be cast as an unsigned int.

Unicode code points vs. hexadecimal values

The Unicode character set assigns a unique code point to each character. For example, the uppercase Latin letter 'A' is assigned the value U+0041. In this case the hexadecimal value is 0x41. VERY IMPORTANT: in this case, there seems to be a straightforward relationship between the Unicode code point and the hexadecimal value. However, this is not generally the case. For example, Cyrillic capital letter A has a Unicode code point U+0410 but a hexadecimal value of 0xD090. I find it easier to ignore the Unicode code point and use the hexadecimal value. Because the characters are processed as four-byte ints, I will frequently include leading zeroes. For example, in the case of the Latin letter 'A', 0x00000041. This provides a reminder that the hex value 0x41 is in the low-order byte.

Encoding Unicode code points as UTF-8

The way that UTF-8 is encoded from the Unicode code point is explained here:
<a href=https://tools.ietf.org/html/rfc3629">Yergeau, F. 2003. UTF-8, a transformation format of ISO 10646. RFC 3629</a>
I have written a function `encode_utf8.c` that takes as its argument a Unicode code point and returns an unsigned int representing the same character in UTF-8 encoding.

Nomeclature of the four bytes as left and right

This tutorial will refer to the leftmost (high-order) byte as the first byte, the second byte from the left as the second byte, the third byte from the left as the third byte, and the fourth byte from the left (the right byte, the low-order byte) as the fourth byte.

Bytes of UTF-8-encoded characters must be processed one byte at a time

Remember that within a four-byte int the different bytes have different numerical values. It is easy to understand this if you think of our base-10 numbering system: in the number "12", the left digit is written as "1", but represents "10". If we want to know whether the digit in the tens place is a "1", we have to look at it in isolation from "12"; otherwise it will be interpreted as "10". In the case of a four-byte int, if we want to know whether the first byte is a "1", that is, whether we have binary 00000001 00000000 00000000 00000000 or hexadecimal 0x01000000, we have to look at just the first byte 00000001 (0x01). This byte taken alone represents "1". If we consider all four bytes together, the first byte represents decimal 16777216 instead of 1. An easy way to evaluate the bytes in isolation from one another is to use bit masking and bit shifting. 

Bit masking.

If we "bitwise and" a four-byte int with the value 0xFF000000, we will keep the value of the first byte and discard the second, third, and fourth bytes. Similarly, if we "bitwise and" a four-byte int with the value 0x00FF0000, we will keep the value of the second byte and discard the first, third, and fourth bytes; and if we "bitwise and" a four-byte int with the value 0x0000FF00, we will keep the value of the third byte and discard the first, second, and fourth bytes; and if we "bitwise and" a four-byte int with the value 0x000000FF, we will keep the value of the fourth byte and discard the first, second, and third bytes. However, remember that the other bits of a four-byte int are zeroed out by masking with 0xFF000000, the high-order byte must be considered in isolation. This is best done by bit shifting.

Bit shifting

If we take the decimal number 16909320, hexadecimal 0x01020408, binary 00000001 00000010 00000100 00001000, and shift it to the right by 24 bits, we get just the left byte 00000001. (We really get 00000000 00000000 00000000 00000001, because 0 bits are padded in from the left to fill the positions that become vacant.) Then we can evaluate this byte in isolation from the other three bytes. An analogous procedure can be followed with the second and third bytes, but there is a complication. If we take this same number and shift it to the right by 16 bits, we get 00000001 00000010. (We really get 00000000 00000000 00000001 00000010.) The complication here is that we get the both second byte and the first byte, so we also have to get rid of the first byte before we can evaluate the second byte. In theory, we could shift it back to the left by eight bits and then back to the right again, but it is easier to mask the unwanted byte first before bit shifing.

Obviously, bit masking should be performed before bit shifting.

In the above example, we shifted the decimal number 16909320, hexadecimal 0x01020408, binary 00000001 00000010 00000100 00001000, to the right by 16 bits, so we got 00000001 00000010, but we really wanted just the second byte 00000010. If we first mask the entire four bytes with 0x00FF0000 we get 00000000 00000010 00000000 00000000. Now, if we shift this masked number to the right by 16 bits, we get 00000000 00000010 (really 00000000 00000000 00000000 00000010). Then, we can evaluate the second byte in isolation. Analogously, we can mask 00000001 00000010 00000100 00001000 by 0x0000FF00 to get 00000000 00000000 00000100 00000000. Shifting this eight bits to the right gives 00000000 00000000 00000100 (really 00000000 00000000 00000000 00000100), so we can evaluate the third byte in isolation. Finally, we can mask 00000001 00000010 00000100 00001000 by 0x000000FF to get 00000000 00000000 00000000 00001000. It is unnecessary to perform bit shifting on this fourth byte, since it is already in the rightmost position, and we can already evaluate the fourth byte in isolation. 

As was mentioned, we do not have to perform any bit masking on the first (left) byte, we simply have to shift it, because the other bytes are lost anyways, and the higher-order bytes are padded with zeroes. Similarly, we do not have to perform bit shifting on the fourth (right) byte; in fact, we should not do so: we simply have to mask it. However, I always bit mask all four bytes at the beginning of my programs, so that they are easily accessible without further processing when needed.  I also like to define two different variables for each byte: an unsigned int and an unsigned char

Putting it all together

The first thing that we have to do is to separate an unsigned int into its component bytes, as explained above. Let us define variables to hold each separate byte. As mentioned above "first" means the left (high-order) byte, and fourth means the right (low-order) byte.

   unsigned char c1; // first char of a UTF-8 sequence
   unsigned char c2; // second char of a UTF-8 sequence
   unsigned char c3; // second third char of a UTF-8 sequence
   unsigned char c4; // fourth char of a UTF-8 sequence
   unsigned int  b1; // first byte of a UTF-8 sequence
   unsigned int  b2; // second byte of a UTF-8 sequence
   unsigned int  b3; // third byte of a UTF-8 sequence
   unsigned int  b4; // fourth byte of a UTF-8 sequence

Outputting UTF-8 encoded text

As was mentioned above, the different bytes of a UTF-8 encoded character have to be processed individually, so the UTF-8 character must be masked and bit shifted before being processed. Here is an example of how to output a UTF-8 character represented by unsigned int u:
```
unsigned int u;

if( is1butf8(u) ) {
   fprintf(stdout, "%c",       (char) ((u & 0x000000FF)      ));
}
if( is2butf8(u) ) {
   fprintf(stdout, "%c%c",     (char) ((u & 0x0000FF00) >>  8),
                               (char) ((u & 0x000000FF)      ));
}
if( is3butf8(u) ) {
   fprintf(stdout, "%c%c%c",   (char) ((u & 0x00FF0000) >> 16),
                               (char) ((u & 0x0000FF00) >>  8),
                               (char) ((u & 0x000000FF)      ));
}
if( is4butf8(u) ) {
   fprintf(stdout, "%c%c%c%c", (char) ((u & 0xFF000000) >> 24),
                               (char) ((u & 0x00FF0000) >> 16),
                               (char) ((u & 0x0000FF00) >>  8),
                               (char) ((u & 0x000000FF)      ));
}
fprintf(stdout, "\n");

(unfinished; will be completed soon)

