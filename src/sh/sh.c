#include "draco.h"
#include "libc.h"

char cmd[256];
char buf[256];

void main()
{
	int i, t;
	for (i = 0; i < 100000; i++);

	while (1)
	{
		printf("# ");
		gets(&cmd);
		
		if (strcmp(&cmd, "uname") == 0)
		{
			uname(&buf);
			printf("%s\n", &buf);
		}
		else if (strcmp(&cmd, "ticks") == 0)
		{
			int tick_count = gettickcount();
			printf("%d ticks\n", tick_count);
		}
		else if (strcmp(&cmd, "boot") == 0)
		{
			printf("system booted from %d\n", getbootdevice());
		}
		else if (strcmp(&cmd, "ps") == 0)
		{
			printf("%d processes:\n", getprocesscount());

			i = 0;
			t = 0;
			while (t < getprocesscount())
			{
				if (isprocess(i))
				{
					getprocessname(i, &buf);
					printf("%s (%d)\n", &buf, getprocessstate(i));
					t++;
				}
				i++;
			}
		}
		else if (strlen(&cmd) == 0);
		else
			printf("%s: command not found\n", &cmd);
	}

	while(1);
}

