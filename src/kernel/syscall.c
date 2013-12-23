#include "draco.h"

#define MODULE_NAME "syscall"

extern process_t proctab[255];
extern uint32 current_task;
extern tasks_running;
extern cons_carret;
extern uint8 cons_attribute;
extern system_info_t system_info;
extern uint32 highest_task;
extern uint32 ticks;
extern uint32 user_ring0_esp;
extern char uname[];

uint32 do_syscall(uint32 eax, uint32 ebx, uint32 ecx, uint32 edx)
{
	uint32 i;
	uint32 *PD, *PT;

	//printf("SYSCALL, eax=%x, ebx=%x, ecx=%x, edx=%x\n", eax, ebx, ecx, edx);

	switch (eax)
	{
		case SYSCALL_UNAME:
		{
			//printf("%s\n", uname);
			memcpy(ebx, uname, strlen(uname));
			*((char*)ebx + strlen(uname)) = '\0';
			break;
		}
		case SYSCALL_GET_IRQ_STATUS:
		{
			return irq_get_status(ebx);
		}
		case SYSCALL_CLEAR_IRQ_STATUS:
		{ 
			irq_clear_status(ebx);
			break;
		}
		case SYSCALL_SET_HEAP_SIZE:	
		{ 
			proc_setheapsize(current_task, ebx); 
			break; 
		}
		case SYSCALL_GET_HEAP_SIZE:
		{
			return proctab[current_task].heap_size;
		}
		case SYSCALL_GET_PID:
		{ 
			return current_task;
		}
		case SYSCALL_GET_PROCESS_NAME:
		{ 
			/* nie mozemy dopuscic kazdego adresu */
			if (ecx > KERNEL_MEMORY)
			{
				memcpy(ecx, &proctab[ebx].name, strlen(&proctab[ebx].name));
				*((char*)ecx + strlen(&proctab[ebx].name)) = '\0';
			}
			break;
		}
		case SYSCALL_GET_PROCESS_COUNT:
		{
			return tasks_running;
		}
		case SYSCALL_GET_PROCESS_STATE:
		{
			return proctab[ebx].state;
		}
		case SYSCALL_SET_PROCESS_STATE:
		{
			proctab[ebx].state = ecx;
			break;
		}
		case SYSCALL_EXIT:
		{ 
			proc_kill(current_task); 
			break; 
		}
		case SYSCALL_PUTS:
		{
			puts(ebx);
			break;
		}
		case SYSCALL_PUTC:
		{
			putc(ebx);
			break;
		}
		case SYSCALL_SET_CARRET_POS:
		{
			cons_carret = ebx;
			break;
		}
		case SYSCALL_GET_CARRET_POS:
		{
			return cons_carret;
		}
		case SYSCALL_SET_CONSOLE_ATTRIBUTE:
		{
			cons_attribute = ebx;
			break;
		}
		case SYSCALL_GET_CONSOLE_ATTRIBUTE:
		{
			return cons_attribute;
		}
		case SYSCALL_GET_MEMORY_USAGE:
		{
			return system_info.used_memory;
		}
		case SYSCALL_SEND_MESSAGE:
		{
			/* asynchronicznie mozna
			if (ebx == current_task)
				return 0;*/

			((message_t*)ecx)->from = current_task;
			ipc_sendmessage(ebx, ecx);
/*
			if (ecx > KERNEL_MEMORY)
			{
				if (proctab[ebx].state == PROC_STATE_WAITING_FOR_MESSAGE)
					proctab[ebx].state = PROC_STATE_READY;

				proctab[current_task].state = PROC_STATE_WORKING;
				memcpy(&proctab[current_task].message, ecx, sizeof(message_t));
				proctab[current_task].message.from = current_task;
				proctab[current_task].message_to = ebx;

				/* ten watek spi, wiec nie mozemy do niego wrocic *
				proctab[current_task].stack = user_ring0_esp;
				schedule();
				write_cr3(proctab[current_task].cr3);

				//zapisujemy jego stos dla r0 w tssie
				user_ring0_esp = proctab[current_task].stack;
				__asm__ __volatile__ ("jmp timer_isr_part2");

				return 1;
			}
			else
			{
				exception_software(0);
				return 0;
			}
*/
			return 1;
		}
		case SYSCALL_GET_MESSAGE:
		{
			if (proctab[current_task].queue_size > 0)
			{
				//printf("getmessage(): %d, %d, %x\n", current_task, proctab[current_task].queue_size, ebx);
				ipc_getmessage(ebx);
				return 1;
			}
			else
				return 0;
/*
			for (i = 0; i < highest_task; i++)
			{
				if (proctab[i].used && proctab[i].state == PROC_STATE_WORKING && proctab[i].message_to == current_task)
				{
					proctab[i].state = PROC_STATE_READY;
					if (ebx > KERNEL_MEMORY)
					{
						memcpy(ebx, &proctab[i].message, sizeof(message_t));
						return 1;
					}
					else
					{
						exception_software(0);
						return 0;
					}
				}
			}
*/
			return 0;
		}
		case SYSCALL_CREATE_SHARED_FRAME:
		{
			return proc_createsharedframe(ebx);
		}
		case SYSCALL_FREE_SHARED_FRAME:
		{
			proc_freesharedframe();
			break;
		}
		case SYSCALL_IS_PROCESS:
		{
			return proctab[ebx].used;
		}
		case SYSCALL_GET_TICKS:
		{
			return ticks;
		}
		case SYSCALL_FIND_PROCESS:
		{
			return proc_find(ebx);
		}
		case SYSCALL_SET_MY_NAME:
		{
			proc_setname(current_task, ebx);
			break;
		}
		case SYSCALL_IRQ_ENABLE:
		{
			irq_enable(ebx);
			break;
		}
		case SYSCALL_IRQ_DISABLE:
		{
			irq_disable(ebx);
			break;
		}
		case SYSCALL_SLEEP:
		{
			if (ebx == SLEEP_SLAVE)
				proctab[current_task].state = PROC_STATE_SLAVE;
			else
				proctab[current_task].state = PROC_STATE_SLEEPING;

			proctab[current_task].sleep_flags = ebx;

			/* ten watek spi, wiec nie mozemy do niego wrocic */
			proctab[current_task].stack = user_ring0_esp;
			schedule();
			write_cr3(proctab[current_task].cr3);

			//zapisujemy jego stos dla r0 w tssie
			user_ring0_esp = proctab[current_task].stack;
			__asm__ __volatile__ ("jmp timer_isr_part2");

			break;
		}
		case SYSCALL_GET_BOOT_DEVICE:
		{
			return system_info.boot_device;
		}
	}
}
