#include "libc.h"
#include "syscall.h"
#include "types.h"

void _wait(int i)
{
	i *= 1000;
	while (i > 0)
		i--;
}

void clear_rect(int lines)
{
	int i;
	systemcall(SYSCALL_SET_CARRET_POS, 0);
	for (i = 0; i < 80 * lines; i++)
		putc(' ');
	systemcall(SYSCALL_SET_CARRET_POS, 0);
}

char buf[128];
char cons_attr;
long cons_carret;

void main()
{
	int i, t, l;

	while(1)
	{
		l++;

		cons_attr = systemcall(SYSCALL_GET_CONSOLE_ATTRIBUTE);
		cons_carret = systemcall(SYSCALL_GET_CARRET_POS);

		systemcall(SYSCALL_SET_CONSOLE_ATTRIBUTE, 25);
		clear_rect(3);
		printf("DracMon system monitor v0.1\n");
		printf("Processes: %d | Mem: %dKB | Ticks: %d\n", 
			systemcall(SYSCALL_GET_PROCESS_COUNT), 
			systemcall(SYSCALL_GET_MEMORY_USAGE),
			systemcall(SYSCALL_GET_TICKS)
		);

		i = 0;
		t = 0;
		while (t < systemcall(SYSCALL_GET_PROCESS_COUNT))
		{
			if (systemcall(SYSCALL_IS_PROCESS, i))
			{
				systemcall(SYSCALL_GET_PROCESS_NAME, i, &buf);
				printf("%s(%d) ", &buf, systemcall(SYSCALL_GET_PROCESS_STATE, i));
				t++;
			}
			i++;
		}

		systemcall(SYSCALL_SET_CONSOLE_ATTRIBUTE, cons_attr);
		systemcall(SYSCALL_SET_CARRET_POS, cons_carret);

		_wait(100);
	}
}
