#include "libc.h"

void * memcpy( void * dest , const void * src , unsigned long num )
{
	unsigned long i;
	unsigned char * uc_dest = (unsigned char*)dest;
	const unsigned char * uc_src = (const unsigned char *)src;
	
	for( i = 0; i < num; i ++ )
		uc_dest[i] = uc_src[i];
		
	return dest;
}
