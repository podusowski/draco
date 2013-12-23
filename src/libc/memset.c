#include "libc.h"

void * memset( void * buffer , int c , unsigned long num )
{
	unsigned long i;
	unsigned char * uc_dest = (unsigned char*)buffer;
	
	for( i = 0; i < num ; i ++ )
		uc_dest[i] = c;
		
	return buffer;
} 
