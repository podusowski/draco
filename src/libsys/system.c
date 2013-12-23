#include "system.h"
#include "syscall.h"
#include "drax.h"

void uname(char * s)
{
	systemcall(SYSCALL_UNAME, s);
}

void * createsharedframe(int pid)
{
	return systemcall(SYSCALL_CREATE_SHARED_FRAME, pid);
}

bool freesharedframe()
{
	return systemcall(SYSCALL_FREE_SHARED_FRAME);
}

int findprocess(char * name)
{
	return systemcall(SYSCALL_FIND_PROCESS, name);
}

bool getmessage(message_t * msg)
{
	return systemcall(SYSCALL_GET_MESSAGE, msg);
}

int getpid()
{
	return systemcall(SYSCALL_GET_PID);
}

int getprocesscount()
{
	return systemcall(SYSCALL_GET_PROCESS_COUNT);
}

bool getprocessname(int pid, char * buf)
{
	return systemcall(SYSCALL_GET_PROCESS_NAME, pid, buf);
}

void setprocessstate(int pid, int state)
{
	systemcall(SYSCALL_SET_PROCESS_STATE, pid, state);
}

int getprocessstate(int pid)
{
	systemcall(SYSCALL_GET_PROCESS_STATE, pid);
}

unsigned long gettickcount()
{
	return systemcall(SYSCALL_GET_TICKS);
}

bool isprocess(int pid)
{
	return systemcall(SYSCALL_IS_PROCESS, pid);
}

bool sendmessage(int pid, message_t * msg)
{
	return systemcall(SYSCALL_SEND_MESSAGE, pid, msg);
}

void setmyname(char * name)
{
	systemcall(SYSCALL_SET_MY_NAME, name);
}

void puts(char * s)
{
	systemcall(SYSCALL_PUTS, s);
}

void putc(char c)
{
	systemcall(SYSCALL_PUTC, c);
}

void gets(char * s)
{
	static message_t msg;
	msg.type = 1;
	msg.lparam = s;
	sendmessage(findprocess("keybd"), &msg);
	sleep(SLEEP_SLAVE);
}

void sleep(unsigned long flags)
{
	systemcall(SYSCALL_SLEEP, flags);
}

void enableirq(int irq)
{
	systemcall(SYSCALL_IRQ_ENABLE, irq);
}

void disableirq(int irq)
{
	systemcall(SYSCALL_IRQ_DISABLE, irq);
}

unsigned int getirqstatus(int irq)
{
	return systemcall(SYSCALL_GET_IRQ_STATUS, irq);
}

void clearirqstatus(int irq)
{
	systemcall(SYSCALL_CLEAR_IRQ_STATUS, irq);
}

int getbootdevice()
{
	return systemcall(SYSCALL_GET_BOOT_DEVICE);
}
