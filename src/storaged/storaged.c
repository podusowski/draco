#include "draco.h"
#include "drax.h"
#include "libc.h"
#include "storaged.h"

#define MAX_DEVS 50000

static dev_t devs[MAX_DEVS];
int highest_device;
int devices_count;

int dev_freeslot()
{
	int i;
	for (i = 0; i < MAX_DEVS; i++)
		if (!devs[i].used)
			return i;
	return -1;
}

void dev_register(char * name, unsigned int did, unsigned int pid)
{
	int i = dev_freeslot();
	char driver_name[32];

	getprocessname(pid, &driver_name);

	printf("storaged: mouting /%s handled by %s\n", name, &driver_name);

	devs[i].used = 1;
	devs[i].did = did;
	devs[i].pid = pid;
	memcpy(&devs[i].name, name, strlen(name));
	*((char*)&devs[i].name + strlen(name)) = '\0';
}

int dev_find(char * path)
{

}

FILE * dev_fopen(char * name, char * mode)
{

}

void dev_fclose(FILE * f)
{

}

void init()
{
	setmyname("storaged");
}

void main()
{
	static message_t msg;
	unsigned long frame;
	request_t * request;

	init();

	while(1)
	{
		sleep(SLEEP_MESSAGE);
		getmessage(&msg);
		
		/* odczytaj request */
		frame = createsharedframe(msg.from);
		request = frame + msg.lparam;
		setprocessstate(msg.from, PROC_STATE_READY);

		switch (request->type)
		{
			case STORAGED_FOPEN:
			{
				break;
			}
			case STORAGED_REGISTER:
			{
				dev_register(frame + request->a, request->b, msg.from);
				break;
			}
		}
		freesharedframe();
	}
}

