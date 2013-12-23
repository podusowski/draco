#include "draco.h"

#define MODULE_NAME "ints"

extern process_t proctab[255];
extern uint32 current_task;
extern uint32 highest_task;

/* tutaj zapiszemy sobie informacje ktory irq zostal wywolany,
   procesy z odpowiednim uprawnieniem beda mogly sobie to sprawdzic
   bajt 0 zawsze bedzie zgaszony bo to zegar */
uint32 irq_status[17];

void init_irq(uint32 pic1, uint32 pic2)
{
   /* send ICW1 */
   outb(PIC1, ICW1);
   outb(PIC2, ICW1);

   /* send ICW2 */
   outb(PIC1 + 1, pic1); /* remap */
   outb(PIC2 + 1, pic2); /*  pics */

   /* send ICW3 */
   outb(PIC1 + 1, 4); /* IRQ2 -> connection to slave */
   outb(PIC2 + 1, 2);

   /* send ICW4 */
   outb(PIC1 + 1, ICW4);
   outb(PIC2 + 1, ICW4);

   /* disable all IRQs */
   //outb(PIC1 + 1, 0xFF);
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);

   /* enable all IRQs */
	//outb(0x21, 0);
	//outb(0xA1, 0);
}

static uint16 irq_mask = 0xFFFF;


void irq_enable(uint8 irq)
{
	irq_mask &= ~(1 << irq);
	if (irq >= 8) irq_mask &= ~(1 << 2);
	
	outb(0x21, irq_mask & 0xFF);
	outb(0xA1, (irq_mask >> 8) & 0xFF);
}

void irq_disable(uint8 irq)
{
	irq_mask |= (1 << irq);
	if ((irq_mask & 0xFF00) == 0xFF00) irq_mask |= (1 << 2);
	
	outb(0x21, irq_mask & 0xFF);
	outb(0xA1, (irq_mask >> 8) & 0xFF);
}

#define IDT_DPL3 0x6000

void setup_int(int i, unsigned long p_hand, unsigned short type, uint32 selector)
{
	gate_desc *idt = (gate_desc *)KERNEL_IDT;
	idt[i].offset_0 = p_hand;
	idt[i].selector = selector;
	idt[i].type = type;
	idt[i].offset_16 = (p_hand >> 16);
}

void init_idt()
{
	int c;
	for(c = 17; c < 256; c++)
	{
		setup_int(c, (uint32)(void*)&unknown_interrupt_isr, IRQ_GATE, 0x08);
	}
   
	setup_int(0, (uint32)(void*)&exception_isr0, TRAP_GATE, 0x08);
	setup_int(1, (uint32)(void*)&exception_isr1, TRAP_GATE, 0x08);
	setup_int(2, (uint32)(void*)&exception_isr2, TRAP_GATE, 0x08);
	setup_int(3, (uint32)(void*)&exception_isr3, TRAP_GATE, 0x08);
	setup_int(4, (uint32)(void*)&exception_isr4, TRAP_GATE, 0x08);
	setup_int(5, (uint32)(void*)&exception_isr5, TRAP_GATE, 0x08);
	setup_int(6, (uint32)(void*)&exception_isr6, TRAP_GATE, 0x08);
	setup_int(7, (uint32)(void*)&exception_isr7, TRAP_GATE, 0x08);
	setup_int(8, (uint32)(void*)&exception_isr8, TRAP_GATE, 0x08);
	setup_int(9, (uint32)(void*)&exception_isr9, TRAP_GATE, 0x08);
	setup_int(10, (uint32)(void*)&exception_isr10, TRAP_GATE, 0x08);
	setup_int(11, (uint32)(void*)&exception_isr11, TRAP_GATE, 0x08);
	setup_int(12, (uint32)(void*)&exception_isr12, TRAP_GATE, 0x08);
	setup_int(13, (uint32)(void*)&exception_isr13, TRAP_GATE, 0x08);
	setup_int(14, (uint32)(void*)&exception_isr14, TRAP_GATE, 0x08);
	setup_int(15, (uint32)(void*)&exception_isr15, TRAP_GATE, 0x08);
	setup_int(16, (uint32)(void*)&exception_isr16, TRAP_GATE, 0x08);

	setup_int(0x21, (uint32)(void*)&irq_isr1, IRQ_GATE, 0x08);
	setup_int(0x22, (uint32)(void*)&irq_isr2, IRQ_GATE, 0x08);
	setup_int(0x23, (uint32)(void*)&irq_isr3, IRQ_GATE, 0x08);
	setup_int(0x24, (uint32)(void*)&irq_isr4, IRQ_GATE, 0x08);
	setup_int(0x25, (uint32)(void*)&irq_isr5, IRQ_GATE, 0x08);
	setup_int(0x26, (uint32)(void*)&irq_isr6, IRQ_GATE, 0x08);
	setup_int(0x27, (uint32)(void*)&irq_isr7, IRQ_GATE, 0x08);
	setup_int(0x28, (uint32)(void*)&irq_isr8, IRQ_GATE, 0x08);
	setup_int(0x29, (uint32)(void*)&irq_isr9, IRQ_GATE, 0x08);
	setup_int(0x2A, (uint32)(void*)&irq_isr10, IRQ_GATE, 0x08);
	setup_int(0x2B, (uint32)(void*)&irq_isr11, IRQ_GATE, 0x08);
	setup_int(0x2C, (uint32)(void*)&irq_isr12, IRQ_GATE, 0x08);
	setup_int(0x2D, (uint32)(void*)&irq_isr13, IRQ_GATE, 0x08);
	setup_int(0x2E, (uint32)(void*)&irq_isr14, IRQ_GATE, 0x08);
	setup_int(0x2F, (uint32)(void*)&irq_isr15, IRQ_GATE, 0x08);
	setup_int(0x30, (uint32)(void*)&irq_isr16, IRQ_GATE, 0x08);

	setup_int(0x80, (uint32)(void*)&int80_isr, USER_CALL_GATE, 0x08);
	setup_int(0x20, (uint32)(void*)&timer_isr, IRQ_GATE, 0x08);

	load_idt();
}

void irq_handler(uint32 irq)
{
	uint32 i;

	for (i = 0; i <= highest_task; i++)
		if (proctab[i].used && proctab[i].state == PROC_STATE_SLEEPING && (proctab[i].sleep_flags & (1 << irq)) == (1 << irq))
		{
			proctab[i].sleep_status = (1 << irq);
			proctab[i].state = PROC_STATE_READY;
		}

	irq_status[irq] ++;
	outb(0x20, 0x20);
	if (irq >= 8)
		outb(0xA0, 0x20);
}

void irq_clear_status(uint32 irq)
{
	irq_status[irq] = 0;
}

uint32 irq_get_status(uint32 irq)
{
	return irq_status[irq];
}


