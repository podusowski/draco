#ifndef __CONSOLE__H
#define __CONSOLE__H

void clrscr();
void putc(uint8 c);
void puts(int8 *str);
void print(int8 *str, int8 attribute);
void console_updatecursor();
void set_carret(uint32 x, uint32 y);

#endif

