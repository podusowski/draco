#ifndef __STORAGED__H__
#define __STORAGED__H__

#define STORAGED_FOPEN 1
#define STORAGED_FCLOSE 2
#define STORAGED_REGISTER 3

typedef struct
{
	char used;
	char name[12];
	unsigned long did;
	unsigned long pid;
} dev_t;

typedef struct
{
	unsigned long type;
	unsigned long a;
	unsigned long b;
	unsigned long c;
	unsigned long result;
} request_t;

typedef int FILE;

#endif //__STORAGED__H__
