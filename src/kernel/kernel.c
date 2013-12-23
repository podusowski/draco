#include "draco.h"

char uname[] = "DRACO compilation "__DATE__" "__TIME__;
system_info_t system_info;
extern char cons_attribute;

void k_main(multiboot_info_t * multiboot_info)
{
	system_info.total_memory = multiboot_info->mem_upper;

	clrscr();

	printf("%s\n", &uname);
	printf("  Copyright 2005-2006 Draco Project\n");
	printf("%dM of extended memory.\n", multiboot_info->mem_upper >> 10);

	cons_attribute = 7;

	init_gdt();
	init_idt();
	init_irq(0x20, 0x28);
	paging_kernel_env(system_info.total_memory / 4);
	paging_init();
	proc_init();
	modules_init(multiboot_info);

	printf("Kernel is initialized.\n");

	sti();

	while(1);
}
