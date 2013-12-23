#include "libc.h"

char * strchr( const char * s, char c )
{
    do
    {
        if ( *s == c )
        {
            return (char *) s;
        }
    } while ( *s++ );
    return 0;
}
