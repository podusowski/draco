#include "draco.h"

#define MODULE_NAME "tasks"

extern system_info_t system_info;

process_t proctab[255];// = (process_t*)KERNEL_PROCTAB; na bssie jest ;]
uint32 current_task = 0;
uint32 highest_task = 0;
uint32 tasks_running = 0;
uint32 ticks = 0;
uint32 max_processes = (KERNEL_MEMORY - KERNEL_PROCTAB) / sizeof(process_t);

tss_t	tss;

sys_desc * sys_gdt = (sys_desc*)KERNEL_GDT;

uint32	* kernel_esp,
		* user_ring0_esp;


void set_clock(uint16 hz)
{
	uint16 val = 1193180 / hz;
	outb(0x36, 0x43);
	outb(val & 0xff, 0x40);
	outb((val >> 8) & 0xff, 0x40);
}

void proc_init()
{
	tss.esp0	= get_esp();
	tss.ss0		= get_ss();

	sys_gdt[5].base_0_15		= (uint32)(&tss)&0x0000FFFF;
	sys_gdt[5].limit			= sizeof(tss);
	sys_gdt[5].base_16_23		= ((uint32)(&tss)&0x00FF0000)>>16;
	sys_gdt[5].dpl_type			= 0x89;
	sys_gdt[5].gav_lim			= 0x90;
	sys_gdt[5].base_24_31		= ((uint32)(&tss)&0xFF000000)>>24;

	__asm__ __volatile__ ("ltr %w0"::"r"(5*8));

	/* stos kernela - w kernel-mode pracujemy na swoim stosie */
	kernel_esp = (uint32*)get_esp();

	//memset(proctab, 0, KERNEL_MEMORY - KERNEL_PROCTAB);

	set_clock(100);
	irq_enable(0);
}


/* musimy miec przynajmniej jeden proces gotowy */
void schedule()
{
	uint32 i;

	while (1)
	{
		current_task ++;
		if (current_task > highest_task)
			current_task = 0;
		if (proctab[current_task].used && proctab[current_task].state == PROC_STATE_READY) 
			return;
	}
}

/* jesli tak, to nie zapisujemy do tabproc stosu */
uint8 scheduler_first_run = 1;

void timer_handler()
{
	ticks ++;

	//zapisujemy stary stos
	if (!scheduler_first_run)
		proctab[current_task].stack = user_ring0_esp;
	else
		scheduler_first_run = 0;
	
	//wybieramy nowy watek
	schedule();

    write_cr3(proctab[current_task].cr3);

	//zapisujemy jego stos dla r0 w tssie
	user_ring0_esp = proctab[current_task].stack;
}

uint32 temp;
uint32 proc_kill(uint32 pid)
{
#ifdef DEBUG
	printf("[ "MODULE_NAME" ] %s (PID %d) killed\n", &proctab[pid].name, pid);
#endif
	temp = pid;

	/* mam nadzieje, ze zmienna pid dalej bedzie obowiazywala jak
	   zmienimy esp, w koncu gcc to do ebp kopiuje */
	ldesp(kernel_esp);
	write_cr3(KERNEL_PAGEDIR);

	proctab[temp].used = 0;
	tasks_running--;

	uint32 i;
	for (i = 0; i < proctab[temp].size; i++)
		paging_free(get_physical_address(proctab[temp].cr3, (i << 12) + KERNEL_MEMORY) >> 12);

	uint32 *PD = (uint32*)proctab[temp].cr3 + 4;
	while (1)
	{
		if (*PD == 0) break;
		paging_free((*PD & 0xFFFFF000) >> 12);
		PD++;
	}
	paging_free((uint32)PD >> 12);

	schedule();

	write_cr3(proctab[current_task].cr3);

	//zapisujemy jego stos dla r0 w tssie
	user_ring0_esp = proctab[current_task].stack;

	/* nie jest to moze sliczne ale skaczac do konca isra
	   timera po prostu skoczymy do nastepnego watku */
	__asm__ __volatile__ ("jmp timer_isr_part2");
}

/* wyszukuje wolny wpis w proctab */
uint32 get_free_pid()
{
	uint32 i;
	/* 0 sux */
	for (i = 1; i < max_processes; i ++)
		if (!proctab[i].used)
			return i;
	return 0;
}

/*
   Pamiec musi byc zmapowana dla procesu, kod zaladowany itp. 

   Funkcja nadaje PID procesowi, wstawia strukture PROCESS do
   tablicy procesow, ustawia stos itp
*/
uint32 proc_add(char * name, uint8 class, uint32 address, uint32 image_size, uint32 e_entry)
{
	uint32 pid = get_free_pid();

	memcpy(&proctab[pid].name, name, strlen(name));
	proctab[pid].name[strlen(name)] = '\0';

	proctab[pid].ipc_buffer = paging_new() << 12;
	proctab[pid].class = class;
	proctab[pid].state = PROC_STATE_READY;
	proctab[pid].cr3 = address;
	//pierw potrzebujemy prawdziwego (fizycznego) wskaznika, potem zmienimy na wirtualny
	proctab[pid].stack = (uint32*)(get_physical_address(proctab[pid].cr3, KERNEL_MEMORY + (((image_size >> 12)+1)<<12) + 0x100));
	proctab[pid].stack3 = (uint32*)(KERNEL_MEMORY + (((image_size >> 12)+1)<<12) + (DEFAULT_STACK_SIZE << 12)); //na koncu strony ze stosami
	//proctab[pid].message_box = (void*)(KERNEL_MEMORY + (((image_size >> 12)+1)<<12) + (DEFAULT_STACK_SIZE << 12));
	proctab[pid].size = (image_size >> 12) + 1 + DEFAULT_STACK_SIZE;

	proctab[pid].stack -- ; *proctab[pid].stack = (8*4)|3; //ss dla r3
	proctab[pid].stack -- ; *proctab[pid].stack = (uint32)proctab[pid].stack3; //esp dla r3

	proctab[pid].stack -- ;
	if (class == PROC_CLASS_A)
		*proctab[pid].stack = 0x3202;  //flagi
	else
		*proctab[pid].stack = 0x0202;  //flagi

	proctab[pid].stack -- ; *proctab[pid].stack = (8*3)|3; //cs dla r3
	proctab[pid].stack -- ; *proctab[pid].stack = e_entry; //eip (kernel+start kodu)
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = 0;
	proctab[pid].stack -- ; *proctab[pid].stack = (8*4)|3;
	proctab[pid].stack -- ; *proctab[pid].stack = (8*4)|3;
	proctab[pid].stack -- ; *proctab[pid].stack = (8*4)|3;
	proctab[pid].stack -- ; *proctab[pid].stack = (8*4)|3;

	proctab[pid].stack = (uint32*)(KERNEL_MEMORY + (((image_size >> 12)+1)<<12) + 0x100) - 17;

	proctab[pid].used = 1;
	proctab[pid].heap_size = 0;

	//uaktualniamy liczniki
	tasks_running ++;
	if (pid > highest_task)
		highest_task = pid;

	return pid;
}

/*
   podajemy rozmiar aplikacji, funkcja zwraca adres
   PD z ktorego cala reszte mozemy odczytac

   aplikacja widzi swoj stos dla kernela ale nie ma to
   znaczenia bo te dane sa wazne tylko wtedy kiedy
   proces spi
*/
uint32 proc_createenv(uint32 image_size)
{
	uint32 application_address;
	uint32 application_env_size;
	uint32 page_tables;

	/* rozmiar obrazu aplikacji bedzie staly, po prostu wrzucamy kod i juz */
	image_size = (image_size >> 12) + 1;

	//rozmiar srodowiska aplikacji W STRONACH
	application_env_size = image_size + DEFAULT_STACK_SIZE;

	//dodajemy miejsce dla PD i PTki
	page_tables = (application_env_size >> 10) + 1;

	/* zmienna page_tables jest liczona BEZ miejsca dla samych
      PTkow bo te nie musza byc zmapowane */
	application_env_size += page_tables + 1; //1 dla PD

	//uint32 address; //aktualny adres do zmapowania
	uint32 i, k, t = 0;
	uint32 *PD = (uint32*)(paging_new() << 12); //page directory
	uint32 *PT = (uint32*)(paging_new() << 12); //pierwsza page table
	uint32 *kernel_PD = (uint32*)KERNEL_PAGEDIR; //page directory kernela

	/* potrzebne do *sharedframe() */
	memset(PD, 0, 4096);

	/* mapujemy kernela (4 PTki, co daje 4*1024 strony - 16MiB) */
	for (k = 0; k < 4; k ++)
		PD[k] = kernel_PD[k];

	/* pierwszy PT do PD */
	PD[k] = (uint32)PT | 7;
	for (i = 0; i < application_env_size; i++)
	{
		PT[t] = (uint32)(paging_new() << 12) | 7; // user mode, read/write, present
		//printf("PT[%d] == %x\n", t, PT[t]);
		t++;
		/* jesli wypelnilismy PTka to robimy nastepny */
		if (t == 1024) 
		{
			k++;
			t = 0;
			PT = (uint32*)(paging_new() << 12);
			PD[k] = (uint32)PT | 7; //user mode, read/write, present
		}
	}

	//printf("k==%d, t==%d\n", k, t);

	return (uint32)PD;
}

/**
	funkcja odpala proces zakladajac, ze jestesmy juz w sti,
	w parametrze podajemy adres do obrazu w przestrzeni
	aktualnego procesu, musimy tu pracowac na roznych
	przestrzeniach - to bedzie hardkor ;]
*/
uint32 proc_exec(char * name, void * image, uint32 size)
{
	/* przesrzen adresowa */
	/** TODO: tu trzeba cr3 kernela */
	void * proc_env = proc_createenv(size); /** TODO: dokladny rozmiar (BSS itp) */
	/** TODO: zaladuj nowy cr3 */
	void * frame = proc_createsharedframe(current_task); /* mam nadzieje, ze zadziala */

	memcpy(frame + image, PROCESS_MEMORY, size); /* kopiujemy obraz do nowej przestrzeni */
	uint32 e_entry = elf_relocate(PROCESS_MEMORY);
	uint32 result = proc_add(name, PROC_CLASS_A, frame, size, e_entry);
	/** TODO: przywroc cr3 */
	return result;
}

uint32 proc_find(char * name)
{
	uint32 i;
	for (i = 1; i <= highest_task; i ++)
		if (proctab[i].used && strcmp(&proctab[i].name, name) == 0)
			return i;
	return 0;
}

void proc_setname(uint32 pid, char * name)
{
	memcpy(&proctab[pid].name, name, strlen(name));
	proctab[pid].name[strlen(name)] = '\0';
}

uint32 proc_setheapsize(uint32 pid, uint32 size)
{
	//printf("Setting heap size for %d from %d to %d pages\n", pid, proctab[pid].heap_size, size);

	/* no potrzeba, co zrobic? :P */
	ldcr3(KERNEL_PAGEDIR);

	uint32	*PT,
			*PD,
			k, t;

	PD = proctab[pid].cr3;

	if (proctab[pid].heap_size == size)
	{
		ldcr3(proctab[pid].cr3);
		return 0;
	}

	k = (proctab[pid].size + (KERNEL_MEMORY >> 12) + DEFAULT_STACK_SIZE) / 1024;
	t = (proctab[pid].size + DEFAULT_STACK_SIZE) % 1024;

	//printf("k==%d, t==%d, proctab[pid].size==%d\n", k, t, proctab[pid].size);

	if (proctab[pid].heap_size < size)
	{
		if (t == 0)
		{
			PT = (uint32*)(paging_new() << 12);
			PD[k + 1] = (uint32)PT | 7;
			t = 0;
		}
		else
		{
			PT = PD[k] & 0xFFFFF000;
			t++;
		}

		while (size > proctab[pid].heap_size)
		{
			proctab[pid].heap_size ++;
			proctab[pid].size ++;
			PT[t] = (uint32)(paging_new() << 12) | 7; // user mode, read/write, present
			t++;
			/* jesli wypelnilismy PTka to robimy nastepny */
			if (t == 1024) 
			{
				k++;
				t = 0;
				PT = (uint32*)(paging_new() << 12);
				PD[k] = (uint32)PT | 7; //user mode, read/write, present
			}
		}
	}
	else
	{
		while (size < proctab[pid].heap_size)
		{
			proctab[pid].heap_size --;
			proctab[pid].size --;

			PT = PD[k] & 0xFFFFF000;
			paging_free((PT[t] & 0xFFFFF000) >> 12);
			//printf("PT[%d] = %x, %x\n", t, PT[t], PT[t] & 0xFFFFF000);
			//printf("paging_free(%d, %x)\n", (PD[t] & 0xFFFFF000) >> 12, PD[t] & 0xFFFFF000);

			PT[t] = 0;
			

			if (t == 0)
			{
				t = 1024;
				k --;
				paging_free((PD[k] & 0xFFFFF000) >> 12);
				PD[k] = 0;
			}
			t --;
		}
	}

	ldcr3(proctab[pid].cr3);
}

/** TODO: proces ma byc mapowany do AKTUALNEGO cr3 */
uint32 proc_createsharedframe(uint32 pid)
{
	//printf("proc_createsharedframe(%d)\n", pid);

	ldcr3(KERNEL_PAGEDIR);

	uint32 * PD = proctab[current_task].cr3;
	uint32 * aPD = proctab[pid].cr3;
	uint32 k, ak, i;

	ak = (KERNEL_MEMORY >> 12) / 1024;
	k =  (SHARED_MEMORY_ADDRESS >> 12) / 1024;
	k += ak;
	

	/* jakis proces juz jest zmapowany *
	if (PD[k] != 0)
	{
		exception_software(1);
		ldcr3(proctab[current_task].cr3);
		return 0;
	}*/

	for (i = 0; i < (proctab[pid].size / 1024) + 1; i++)
	{
		//printf("proc_createsharedframe(%d): PD[%d] = aPD[%d] = %x\n", pid, k, ak, aPD[ak]);
		PD[k] = aPD[ak];
		k++;
		ak++;
	}

	ldcr3(proctab[current_task].cr3);

	//printf("%x\n", SHARED_MEMORY_ADDRESS);
	return SHARED_MEMORY_ADDRESS;
}

void proc_freesharedframe()
{
	ldcr3(KERNEL_PAGEDIR);

	uint32 * PD = proctab[current_task].cr3;
	uint32 k = (SHARED_MEMORY_ADDRESS >> 12) / 1024;
	k += (KERNEL_MEMORY >> 12) / 1024;

	while (PD[k] != 0)
	{
		PD[k] = 0;
		k++;
	}

	ldcr3(proctab[current_task].cr3);
}
