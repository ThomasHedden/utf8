#include <stdio.h>
#include <stdbool.h>

int isutf8bom(unsigned int u) {
   if(u == 0xEFBBBF)
      return true;
   return false;
}
