#include "draco.h"
#include "libc.h"

#define KBD_SPECIAL 200
#define ENTER 10
#define F1 201
#define F2 202
#define F3 203
#define F4 204
#define F5 205
#define F6 206
#define F7 207
#define F8 208
#define F9 209
#define F10 210
#define F11 211
#define F12 212
#define PAUSE 213

char scancode_ascii[0x100] = {
   KBD_SPECIAL, KBD_SPECIAL,
   '1','2','3','4','5','6','7','8','9','0','-','=',
   27, KBD_SPECIAL,
   'q','w','e','r','t','y','u','i','o','p','[',']','\n',KBD_SPECIAL,
   'a','s','d','f','g','h','j','k','l',';','\'',KBD_SPECIAL,'\\',
   '<','z','x','c','v','b','n','m',',','.','/',KBD_SPECIAL,KBD_SPECIAL,KBD_SPECIAL,
   ' ',KBD_SPECIAL,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,PAUSE,KBD_SPECIAL,KBD_SPECIAL,
};

int shift = 0;
int escaped = 0;

int buffering = 0;
char * buffer;

message_t msg;

char readkey()
{
	char key;
	char kbd_scancode = inb(0x60);

	// FIXME: for some reason, we need to make some syscall to make this work,
	//        otherwise the keyboard buffer gets full
	getpid();

	if (escaped) kbd_scancode += 256;

	switch (kbd_scancode) 
	{
		case 0x2a: shift = 1; break;
		case 0xaa: shift = 0; break;
		case 0xe0: escaped = 1; break;
		default:
			if (kbd_scancode & 0x80) 
			{
				return 0;
			} 
			else 
			{
				return scancode_ascii[kbd_scancode];
			}
		break;
	}
}

void do_gets(int pid, char * buffer)
{
	void * frame = createsharedframe(pid);
	char key = 0;
	static int i, k;

	buffer = (char*)((unsigned long)buffer + (unsigned long)frame);

	i = 0;
	while (key != '\n')
	{
		sleep(SLEEP_IRQ1);
		for (k = getirqstatus(1); k > 0; k --)
		{
			key = readkey();
			if (key == 0) continue;
			if (key == 27)
			{
				buffer[i] = 0;
				i --;
			}
			if (i < 0) 
			{
				i = 0;
				continue;
			}
			putc(key);
			buffer[i] = key;
			i ++;
		}
		clearirqstatus(1);
	}
	buffer[i == 0 ? i : i - 1] = 0;

	setprocessstate(pid, PROC_STATE_READY);

	freesharedframe();
}

void main()
{
	enableirq(1);

	printf("keybd: standard keyboard driver\n");
	setmyname("keybd");

	while(1)
	{
		sleep(SLEEP_MESSAGE);

		getmessage(&msg);
		if (msg.type == 1)
		{
			do_gets(msg.from, msg.lparam);
		}
	}
}

