#ifndef __DRAX__H__
#define __DRAX__H__

typedef struct {
	unsigned long 
			type, 
			lparam,
			hparam,
			from;
} message_t;

typedef int bool;

#define PROC_CLASS_A	1
#define PROC_CLASS_B	2

#define PROC_STATE_READY	1
#define PROC_STATE_SLEEPING	2
#define PROC_STATE_SLAVE	4
#define PROC_STATE_KILLED	5

/* te wartosci musza byc stale */
#define SLEEP_IRQ1 (1 << 1)
#define SLEEP_IRQ2 (1 << 2)
#define SLEEP_IRQ3 (1 << 3)
#define SLEEP_IRQ4 (1 << 4)
#define SLEEP_IRQ5 (1 << 5)
#define SLEEP_IRQ6 (1 << 6)
#define SLEEP_IRQ7 (1 << 7)
#define SLEEP_IRQ8 (1 << 8)
#define SLEEP_IRQ9 (1 << 9)
#define SLEEP_IRQ10 (1 << 10)
#define SLEEP_IRQ11 (1 << 11)
#define SLEEP_IRQ12 (1 << 12)
#define SLEEP_IRQ13 (1 << 13)
#define SLEEP_IRQ14 (1 << 14)
#define SLEEP_IRQ15 (1 << 15)
#define SLEEP_IRQ16 (1 << 16)

#define SLEEP_MESSAGE (1 << 17)
#define SLEEP_SLAVE (1 << 18)

#endif // __DRAX__H__
