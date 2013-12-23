#ifndef __LIBC__H
#define __LIBC__H

#include "limits.h"

char * 	ultoa( unsigned long val , int base );
int		strlen( char * str );
void *	memcpy( void * dest , const void * src , unsigned long num );
int 	sprintf( char * dest , const char * fmt , ... );
void 	printf(const char * fmt, ...);
void * 	memset( void * buffer , int c , unsigned long num );
int 	strcmp( const char * s1, const char * s2 );
char *	strchr( const char * s, char c );

#endif
 
