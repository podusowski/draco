#include "libc.h"
#include "stdarg.h"

void printf(const char * fmt, ...)
{
	va_list	ap;
	int i_fmt;

	va_start(ap, fmt);

	for (i_fmt = 0; i_fmt < INT_MAX;)
	{
		if (fmt[i_fmt] == '%')
		{
			i_fmt++;
			
			char 		*argchptr;
			unsigned long	argul;
			unsigned char	argch;

			switch (fmt[i_fmt])
			{
			case 's':
				argchptr = va_arg( ap, char* );
				puts(argchptr);
				i_fmt++;
				break;
				
			case 'X':
			case 'x':
				argul = va_arg( ap , unsigned long );
				argchptr = ultoa( argul , 16 );
				puts(argchptr);
				i_fmt++;
				break;
				
			case 'c':
				argch = va_arg( ap , char );
				putc(argch);
				i_fmt++;
				break;
				
			case 'd':
				argul = va_arg( ap , unsigned long );
				argchptr = ultoa( argul , 10 );
				puts(argchptr);
				i_fmt++;
				break;
				
			default:
				break;
			}
		} 
		else if( fmt[i_fmt] == '\0' )
		{
			break;
		} 
		else 
		{
			putc(fmt[i_fmt]);
			i_fmt++;
		}
	}
} 
