#include "libc.h"
#include "syscall.h"
#include <drax.h>
#include "types.h"

message_t msg;
char name[32];
int addr;
int floppyd = 0;
char buf[512];
char str[] = "Cos co zostanie zapisane na dyskietke";
void _wait(int i)
{
	i *= 1000;
	while (i > 0)
		i--;
}

void main()
{
	printf("Test Module for Draco\n");
	getprocessname(getpid(), &name);
	printf("My name is %s and my PID is %d\n", &name, getpid());

	if (getpid() == 3)
	{
		while(floppyd < 1)
			floppyd = findprocess("floppyd");


		/* zapisujemy cos */
		msg.type = 2;
		msg.lparam = 0;
		msg.hparam = &str;
		sendmessage(floppyd, &msg);

		//musimy odebrac wiadomosc bo sie zablokuje
		while(!getmessage(&msg));

		/* odczytujemy */
		msg.type = 1;
		msg.lparam = 0;
		msg.hparam = &buf;
		sendmessage(floppyd, &msg);

		//puts(&buf);
	}

	while(1)
	{
		sleep(SLEEP_MESSAGE);
		if (getmessage(&msg))
		{
			printf("Floppy buffer from floppyd: %s\n", &buf);
		}
		_wait(100);
	}
}
