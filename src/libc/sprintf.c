#include <stdarg.h>
#include "libc.h"

int sprintf( char * dest , const char * fmt , ... )
{
	va_list		ap;
	int		i_dest, i_fmt, i;
	
	va_start( ap , fmt );
	
	for( i_dest = 0, i_fmt = 0; i_fmt < INT_MAX; )
	{
		if( fmt[i_fmt] == '%' )
		{ 		
			i_fmt++;
			
			char 		*argchptr;
			unsigned long	argul;
			unsigned char	argch;

			switch( fmt[i_fmt] )
			{
			case 's':
				argchptr = va_arg( ap, char* );
				memcpy( &dest[i_dest], argchptr, strlen( argchptr ) );
				i_dest += strlen( argchptr );
				i_fmt++;
				break;
				
			case 'X':
			case 'x':
				argul = va_arg( ap , unsigned long );
				argchptr = ultoa( argul , 16 );
				memcpy( &dest[i_dest], argchptr, strlen( argchptr ) );
				i_dest += strlen( argchptr );
				i_fmt++;
				break;
				
			case 'c':
				argch = va_arg( ap , int );
				dest[i_dest] = argch;
				i_dest++;
				i_fmt++;
				break;
				
			case 'd':
				argul = va_arg( ap , unsigned long );
				argchptr = ultoa( argul , 10 );
				memcpy( &dest[i_dest], argchptr, strlen( argchptr ) );
				i_dest += strlen( argchptr );
				i_fmt++;
				break;
				
			default:
				break;
			}
				
		} else if( fmt[i_fmt] == '\0' )
		{
			dest[i_dest] = '\0';
			break;
		} else
		{
			dest[i_dest] = fmt[i_fmt];
			i_fmt  ++;
			i_dest ++;
		}
	}
	
	return i_dest;
}

