#ifndef __DRACO__H
#define __DRACO__H

#define KERNEL_ADDRESS			0x100000 //1mb
#define KERNEL_MEMORY			0x1000000 //16mb
#define PROCESS_MEMORY			0x1000000
#define KERNEL_PAGEDIR			0x200000 //2mb
/* nie wiemy ile bedzie pagetabow wiec musimy
   zalozyc, ze 1024 (1mb?) */
#define KERNEL_PAGETABS		0x300000 //3mb
#define KERNEL_GDT			0x400000 //4mb 
#define KERNEL_IDT			0x400400 //4mb 1kb
#define KERNEL_PAGEBMP		0x500000 /* 5mb */
#define KERNEL_PROCTAB		0x600000 /* 6mb-16mb */

#define KERNEL_DMA_BUFFER	0x80000

#define SHARED_MEMORY_ADDRESS	0xC0000000

/* Prcoesses def's */
#define DEFAULT_STACK_SIZE 2
#define DEFAULT_MSGBOX_SIZE 1

#include "drax.h"

#include "libc.h"
#include "types.h"
#include "inlines.h"
#include "kernel.h"
#include "gdt.h"
#include "ints.h"
#include "paging.h"
#include "proc.h"
#include "syscall.h"
#include "console.h"
#include "exception.h"
#include "ipc.h"
#include "elf.h"


#endif
