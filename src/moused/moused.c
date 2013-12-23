#include "draco.h"
#include "libc.h"
#include "moused.h"

uint8 mouse_cycle=0;    //unsigned char
int8 mouse_byte[3];    //signed char
int8 mouse_x=0;        //signed char
int8 mouse_y=0;        //signed char

inline void mouse_wait(uint8 a_type) //unsigned char
{
  uint32 _time_out = 100000;
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(uint8 a_write)
{
  mouse_wait(1);
  outb(0x64, 0xD4);
  mouse_wait(1);
  outb(0x60, a_write);
}

uint8 mouse_read()
{
  //Get's response from mouse
  mouse_wait(0);
  return inb(0x60);
}

void init()
{
	printf("moused: init\n");

	uint8 _status;  //unsigned char

	//Enable the auxiliary mouse device
	mouse_wait(1);
	outb(0x64, 0xA8);
 
	//Enable the interrupts
	mouse_wait(1);
	outb(0x64, 0x20);
	mouse_wait(0);
	_status=(inb(0x60) | 2);
	mouse_wait(1);
	outb(0x64, 0x60);
	mouse_wait(1);
	outb(0x60, _status);
 
  //Tell the mouse to use default settings
	mouse_write(0xF6);
	mouse_read();  //Acknowledge
 
	//Enable the mouse
	mouse_write(0xF4);
	mouse_read();  //Acknowledge
}

void main()
{
	//while(1);
	systemcall(SYSCALL_CLEAR_IRQ_STATUS, 12);
	systemcall(SYSCALL_IRQ_ENABLE, 12);
	init();

	setmyname("moused");

	while(1)
	{
		sleep(1 << 12);
		if (systemcall(SYSCALL_GET_IRQ_STATUS, 12))
		{
			switch(mouse_cycle)
			{
			case 0:
				mouse_byte[0] = inb(0x60);
				mouse_cycle++;
				break;
			case 1:
				mouse_byte[1] = inb(0x60);
				mouse_cycle++;
				break;
			case 2:
				mouse_byte[2] = inb(0x60);
				mouse_x += mouse_byte[1];
				mouse_y += mouse_byte[2];
				mouse_cycle=0;
				break;
			}
			printf("%dx%d\n", mouse_x, mouse_y);
			systemcall(SYSCALL_CLEAR_IRQ_STATUS, 12);
		}
	}
}

