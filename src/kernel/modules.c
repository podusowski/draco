#include "draco.h"

#define MODULE_NAME "modules"

void modules_init(multiboot_info_t * multiboot_info)
{
	printf("[ "MODULE_NAME" ] loading: ");

	uint32 i;
	void * proc_mem;

	module_t *modules = (module_t *)multiboot_info->mods_addr;
	for (i = 0; i < multiboot_info->mods_count; i++)
	{

		/** ELF EXEC */
		if (*((char *)(modules[i].mod_start)) == 0x7f)
		{
			elf_dump(modules[i].mod_start);
			printf("%s ", modules[i].string);
			elf_exec(modules[i].string, modules[i].mod_start, modules[i].mod_end - modules[i].mod_start);
			continue;
		}

		printf("%s ", modules[i].string);

		proc_mem = proc_createenv(modules[i].mod_end - modules[i].mod_start);
		memcpy(get_physical_address(proc_mem, KERNEL_MEMORY), modules[i].mod_start, modules[i].mod_end - modules[i].mod_start);
		proc_add(modules[i].string, PROC_CLASS_A, proc_mem, modules[i].mod_end - modules[i].mod_start, KERNEL_MEMORY);
	}
	printf("\n");
}
