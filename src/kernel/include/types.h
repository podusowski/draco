#ifndef __TYPES__H
#define __TYPES__H

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;

typedef char   int8;
typedef short  int16;
typedef long   int32;

typedef uint32 PTE;
typedef uint32 PDE;

typedef struct {
	uint32 flags;
	uint32 mem_lower;
	uint32 mem_upper;
	uint32 boot_device;
	uint32 cmdline;
	uint32 mods_count;
	uint32 mods_addr;
	uint32 syms[4];
	uint32 mmap_length;
	uint32 mmap_addr;

/* Drive Info buffer */
  uint32 drives_length;
  uint32 drives_addr;

  /* ROM configuration table */
  uint32 config_table;

  /* Boot Loader Name */
  uint32 boot_loader_name;

  /* APM table */
  uint32 apm_table;
} multiboot_info_t;

typedef struct module
{
	uint32 mod_start;
	uint32 mod_end;
	uint32 string;
	uint32 reserved;
} module_t;

typedef struct {
	uint16 offset_0;
	uint16 selector;
	uint16 type;
	uint16 offset_16;
} gate_desc;

typedef struct  {
  uint16 limit;
  uint16 base_0_15;
  uint8 base_16_23;
  uint8 dpl_type;
  uint8 gav_lim;
  uint8 base_24_31;
} sys_desc;

typedef struct {
	uint32 backlink;
	uint32 esp0;
	uint32 ss0;
	uint32 esp1;
	uint32 ss1;
	uint32 esp2;
	uint32 ss2;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint32 es;
	uint32 cs;
	uint32 ss;
	uint32 ds;
	uint32 fs;
	uint32 gs;
	uint32 ldt;
	uint32 bmoffset;
} tss_t;

typedef struct {
	uint8 used;

	char name[32];
	uint8 class;
	uint32 *stack;
	uint32 *stack3;
	void * ipc_buffer;
	uint32 queue_begin, queue_end, queue_size;

	/*uint32 message_to;
	message_t message;
	callbacks_t callbacks;*/

	uint32 cr3;
	uint8 state;
	uint32 sleep_flags;
	uint32 sleep_status;
	uint32 image_size; // rozmiar aplikacji (dane+kod)
	uint32 stack_size;
	uint32 heap_size;
	uint8 irq;

	/* rozmiar srodowiska w stronach (bez PD i PT) */
	uint32 size;
} process_t;

typedef struct {
   void *next;
   uint8 used;
   uint32 size;
   uint8 data;
} heap_block_t;

typedef struct {
   heap_block_t *first_block;
   uint32 size;
} heap_info_t;

typedef struct {
	uint32 total_memory;
	uint32 used_memory;
	uint32 boot_device;
} system_info_t;

#endif
