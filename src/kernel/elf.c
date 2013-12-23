#include "draco.h"


char * elf_sectionName(void * image, int section)
{
	Elf32_Ehdr * header = image;
	Elf32_Shdr * sections = image + header->e_shoff;
	return image + sections[header->e_shstrndx].sh_offset + sections[section].sh_name;
}

int elf_findSection(void * image, char * name)
{
	Elf32_Ehdr * header = image;
	Elf32_Shdr * sections = image + header->e_shoff;
	int i;

	for (i = 0; i < header->e_shnum; i++)
		if (strcmp(elf_sectionName(image, i), name) == 0)
			return i;

	return -1;
}

Elf32_Sym * elf_findSymbol(void * image, char * name)
{

}

uint32 elf_imageSize(void * image)
{
	Elf32_Ehdr * header = image;
	Elf32_Shdr * sections = image + header->e_shoff;
	uint32 result = 0;
	int i;

	for (i = 0; i < header->e_shnum; i++)
		if (sections[i].sh_addr > 0)
			result += sections[i].sh_size;

	return result;
}

/** zwraca eip, jak NULL to jakis blad */
void * elf_relocate(void * image)
{
	return 0;
}

void elf_exec(char * name, void * image, uint32 size)
{
	int i, t, reloc_size;
	void * proc_mem = proc_createenv(elf_imageSize(image));
	void * proc_offset = KERNEL_MEMORY;
	Elf32_Ehdr * header = image;
	Elf32_Shdr * sections = image + header->e_shoff;


	memcpy(get_physical_address(proc_mem, PROCESS_MEMORY), image, elf_imageSize(image));


	for (i = 1; i < header->e_shnum; i++)
	{
		printf("%s ", elf_sectionName(image, i));

		if (sections[i].sh_type == SHT_RELA)	/* RELA */
		{
			for (t = 0; t < sections[i].sh_size / sizeof(Elf32_Rela); t++);
				/* do_reloc */
		}
		else if(sections[i].sh_type == SHT_REL) /* REL */
		{
			for (t = 0; t < sections[i].sh_size / sizeof(Elf32_Rel); t++);
				/* do_reloc */
		}
		else continue;
	}

	//memcpy(get_physical_address(proc_mem, KERNEL_MEMORY), image, size);
	proc_add(name, PROC_CLASS_A, proc_mem, size, header->e_entry);
}


void elf_dump(void * image)
{
	int i;

	printf("\nelfinfo:\n");

	Elf32_Ehdr * header = image;

	printf("e_ident[EI_CLASS]: %s\n", header->e_ident[EI_CLASS] == ELFCLASS32 ? "ELFCLASS32" : "ELFCLASS64");
	printf("e_entry: 0x%x\n", header->e_entry);
	printf("e_type: %s\n", header->e_type == ET_EXEC ? "EXEC" : "NON-EXEC");
	printf("e_phoff: 0x%x\n", header->e_phoff);
	printf("e_phnum: %d\n", header->e_phnum);
	printf("e_shoff: 0x%x\n", header->e_shoff);
	printf("e_shnum: %d\n", header->e_shnum);
	printf("e_shstrndx: %d\n", header->e_shstrndx);

	
	Elf32_Shdr * sections = image + header->e_shoff;

	printf("Sections dump (sh_offset:sh_size:sh_addr):\n");
	for (i = 0; i < header->e_shnum; i++)
	{
		printf("  %s (0x%x:0x%x:0x%x)\n", elf_sectionName(image, i), sections[i].sh_offset, sections[i].sh_size, sections[i].sh_addr);
	}
/*
	Elf32_Phdr * p_headers = image + header->e_phoff;

	printf("Program header dump:\n");
	for (i = 0; i < header->e_phnum; i++)
	{
		printf("  %s\n", elf_sectionName(image, i));
		printf("  p_offset: 0x%x\n", p_headers[i].p_offset);
	}
	*/
}
