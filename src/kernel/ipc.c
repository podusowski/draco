#include "draco.h"

#define QUEUE_SIZE (4096/sizeof(message_t))

extern process_t proctab[];
extern current_task;

void ipc_sendmessage(uint32 pid, message_t * msg)
{
	ldcr3(KERNEL_PAGEDIR);
	if( proctab[pid].queue_size < QUEUE_SIZE )
	{
		memcpy(proctab[pid].ipc_buffer + (proctab[pid].queue_begin * sizeof(message_t)), get_physical_address(proctab[current_task].cr3, msg), sizeof(message_t));

		if (proctab[pid].queue_begin == QUEUE_SIZE-1)
			proctab[pid].queue_begin = 0;
		else
			proctab[pid].queue_begin ++;

		proctab[pid].queue_size  ++;
	}

	if (proctab[pid].state == PROC_STATE_SLEEPING && (proctab[pid].sleep_flags & SLEEP_MESSAGE) == SLEEP_MESSAGE)
	{
		proctab[pid].sleep_status = SLEEP_MESSAGE;
		proctab[pid].state = PROC_STATE_READY;
	}

	ldcr3(proctab[current_task].cr3);
}


void ipc_getmessage(message_t * buf)
{
	ldcr3(KERNEL_PAGEDIR);
	if (proctab[current_task].queue_size > 0)
	{
		memcpy(get_physical_address(proctab[current_task].cr3, buf), proctab[current_task].ipc_buffer + (proctab[current_task].queue_end * sizeof(message_t)), sizeof(message_t));

		if (proctab[current_task].queue_end == QUEUE_SIZE - 1)
			proctab[current_task].queue_end = 0;
		else
			proctab[current_task].queue_end ++;

		proctab[current_task].queue_size --;
	}
	ldcr3(proctab[current_task].cr3);
}
