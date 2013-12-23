#include "libc.h"

int strlen( char * str )
{
   int len;
   for( len = 0; len < INT_MAX; len ++ )
      if( str[len] == '\0' )
         return len;
} 

