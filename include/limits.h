#ifndef	_LIMITS_H
#define	_LIMITS_H

/* Number of bits in a `char'.	*/
#define	CHAR_BIT	8

/* Maximum length of any multibyte character in any locale.
   Locale-writers should change this as necessary.  */
#define	MB_LEN_MAX	1

/* Minimum and maximum values a `signed char' can hold.  */
#define	SCHAR_MIN	(-128)
#define	SCHAR_MAX	127

/* Maximum value an `unsigned char' can hold.  (Minimum is 0.)  */
#ifdef	__STDC__
#define	UCHAR_MAX	255U
#else
#define	UCHAR_MAX	255
#endif

/* Minimum and maximum values a `char' can hold.  */
#ifdef __CHAR_UNSIGNED__
#define	CHAR_MIN	0
#define	CHAR_MAX	UCHAR_MAX
#else
#define	CHAR_MIN	SCHAR_MIN
#define	CHAR_MAX	SCHAR_MAX
#endif

/* Minimum and maximum values a `signed short int' can hold.  */
#define	SHRT_MIN	(-32768)
#define	SHRT_MAX	32767

/* Maximum value an `unsigned short int' can hold.  (Minimum is 0.)  */
#define	USHRT_MAX	65535

/* Minimum and maximum values a `signed int' can hold.  */
#define	INT_MIN	(- INT_MAX - 1)
#define	INT_MAX	2147483647

/* Maximum value an `unsigned int' can hold.  (Minimum is 0.)  */
#ifdef	__STDC__
#define	UINT_MAX	4294967295U
#else
#define	UINT_MAX	4294967295
#endif

/* Minimum and maximum values a `signed long int' can hold.  */
#define	LONG_MIN	INT_MIN
#define	LONG_MAX	INT_MAX

/* Maximum value an `unsigned long int' can hold.  (Minimum is 0.)  */
#define	ULONG_MAX	UINT_MAX

#endif	/* limits.h  */
