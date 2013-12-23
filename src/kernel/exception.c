#include "draco.h"

#define MODULE_NAME "EXCEPTION"

extern process_t proctab[];
extern uint32 current_task;

char * hardware_exceptions[] = 
{
	"Divide Error",
	"Debug",
	"NMI Interrupt",
	"Breakpoint",
	"Overflow",
	"BOUND Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"x87 FPU Floating-Point Error",
	"Alignment Check",
	"Machine-Check",
	"SIMD Floating-Point"
};

char * software_exceptions[] =
{
	"Invalid Pointer",
	"Some process already mapped while createsharedframe()",
	"Trying to call class A function"
};

void exception_hardware(uint32 nr, uint32 cr2)
{
	if (nr == 14)
		printf("page fault while accessing %x by %s (PID %d)\n", cr2, &proctab[current_task].name, current_task);
	else
		printf("%s (PID %d) raised an exception \"%s\"\n", &proctab[current_task].name, current_task, hardware_exceptions[nr]);

	proc_kill(current_task);
}

void exception_software(uint32 nr)
{
	printf("%s (PID %d) raised an exception \"%s\"\n", &proctab[current_task].name, current_task, software_exceptions[nr]);
}
