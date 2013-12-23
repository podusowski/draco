#ifndef __INTS__H
#define __INTS__H

extern gate_desc idt[256];

void unknown_interrupt_isr();
void int80_isr();
void timer_isr();
void floppy_isr();
void keyb_isr();

void exception_isr0();
void exception_isr1();
void exception_isr2();
void exception_isr3();
void exception_isr4();
void exception_isr5();
void exception_isr6();
void exception_isr7();
void exception_isr8();
void exception_isr9();
void exception_isr10();
void exception_isr11();
void exception_isr12();
void exception_isr13();
void exception_isr14();
void exception_isr15();
void exception_isr16();

//void irq_isr0();
void irq_isr1();
void irq_isr2();
void irq_isr3();
void irq_isr4();
void irq_isr5();
void irq_isr6();
void irq_isr7();
void irq_isr8();
void irq_isr9();
void irq_isr10();
void irq_isr11();
void irq_isr12();
void irq_isr13();
void irq_isr14();
void irq_isr15();
void irq_isr16();

#define PIC1 0x20
#define PIC2 0xA0
#define ICW1 0x11
#define ICW4 0x01

#define TRAP_GATE 0x8F00
#define IRQ_GATE 0x8E00
#define CALL_GATE 0x8C00
#define USER_CALL_GATE 0xEE00

void init_irq(uint32 pic1, uint32 pic2);
void setup_int(int i, unsigned long p_hand, unsigned short type, uint32 selector);
void load_idt();

void irq_handler(uint32 irq);
void irq_clear_status(uint32 irq);
uint32 irq_get_status(uint32 irq);
void irq_disable(uint8 irq);
void irq_enable(uint8 irq);

#endif
