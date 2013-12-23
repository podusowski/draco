#include <stdarg.h>
#include "draco.h"

#define MODULE_NAME "console"

uint32 cons_carret = 0;
uint8 cons_attribute = 8;

void clrscr()
{
	uint8 *vga = (uint8*)0xB8000;
	for (; vga <= (uint8*)0xB8000 + 4000; vga ++)
	{
		*vga = 0;
	}
}

void putc(uint8 c)
{
	uint8 *vga = 0xB8000;

	vga += cons_carret * 2;

	if (c == 27)
	{
		vga -= 2;
		*vga = 0;
		*(vga + 1) = cons_attribute;
		cons_carret --;
	}
	else
	if (c == 10)
	{
		cons_carret += 80 - ( cons_carret % 80 );
	} 
	else
	{
		*vga = c;
		*(vga + 1) = cons_attribute;
		cons_carret ++;
	}

	vga += 2;
	*vga = ' ';
	*(vga + 1) = cons_attribute;


	if (cons_carret / 80 > 24)
	{
		cons_carret = 24 * 80;
		memcpy((uint8 *)0xB8000, (uint8 *)0xB8000 + 160, 160 * 24);

		uint8 i;
		for (i = 0; i <= 160; i ++)
		{
			*(uint8*)(0xB8000 + 3840 + i) = 0;
		}
	}
	console_updatecursor();
}

void puts(int8 *str)
{
	while(*str != 0)
	{
		putc(*str);
		str++;
	}
}

void print(int8 *str, int8 attribute)
{
	cons_attribute = attribute;
	while(*str != 0)
	{
		putc(*str);
		str++;
	}
	putc('\n');
}

void console_updatecursor()
{
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8)(cons_carret & 0xFF));

	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8)((cons_carret >> 8) & 0xFF));
}

void set_carret(uint32 x, uint32 y)
{
	cons_carret = (y * 80) + x;
	console_updatecursor();
}

