#include "draco.h"

#define MODULE_NAME "paging"

extern system_info_t system_info;

/* 
   wlacza stronicowanie i zeruje pagebmp, przed wywolaniem
   powinnismy zmapowac pamiec dla jadra
*/
void paging_init() 
{
	uint8 *PB = (uint8*)KERNEL_PAGEBMP;

	ldcr3(KERNEL_PAGEDIR);
	ldcr0(get_cr0() | 0x80000000);

	/* pewnie na prawdziwym kompie bedzie wyczyszczona ale dla pewnosci
      mozemy ja wyczyscic sami, zwlaszcza, ze dlugo to nie potrwa */
	memset(KERNEL_PAGEBMP, 0, 1024*128);

	/* ustawiamy strony kernela (16MB) jako zajete */
	memset(KERNEL_PAGEBMP, 0xFF, 512);

	system_info.used_memory = 16 << 10;
}

/* mapuje cala pamiec dla jadra, ilosc pamieci podawana
   w parametrze pages */
void paging_kernel_env(uint32 pages)
{
	uint32 address = 0; //aktualny adres do zmapowania
	uint32 i, k;
	uint32 pts; //ilosc tabel stron
	uint32 *PT = (uint32*)KERNEL_PAGETABS; //page table
	uint32 *PD = (uint32*)KERNEL_PAGEDIR; //page directory

	pts = pages >> 10;

	/* tworzymy page directory */
	for (i = 0; i < pts; i ++)
	{
		PT = (uint32*)KERNEL_PAGETABS + (i << 12);
		for (k = 0; k < 1024; k ++)
		{
			PT[k] = address | 7; // user mode, read/write, present
			address += 4096; // nastepna strona ...
		}
		//wypelniamy page dir
		PD[i] = (uint32)PT | 7; //user mode, read/write, present
	}

	/* nie wiem czy nie powinnismy wypelnic calego page dir, zobaczymy
	   w koncu prostota rlz */
}

/*
   Funkcja wyszukuje wolna strone, zwraca jej _numer_ i zapala jej bit - W.

   DOTO: w tej wersji bitmapa jest na sztyno ustawiona na milion stron (4GB)
         bootloader powinien dostarczyc nam informacje o pamieci i wg tego
         ew wywalac 'out of memory'
*/
uint32 paging_new()
{
	uint8 *pagebmp = (uint8 *)KERNEL_PAGEBMP;
	uint32 i, p;
	for (i = 0; i <= 1024*128; i ++, pagebmp ++)
	{
		/* nie jestem pewien czy to ma sens, chociaz
			mysle, ze asmowe cmp jest szybsze niz moja
			petelka */
		if (*pagebmp < 0xFF)
		{
			for (p = 0; p <= 7; p ++)
			{
				if (((*pagebmp >> p) & 1) == 0)
				{
					system_info.used_memory += 4;
					*pagebmp |= 1 << p;
					return (i*8) + p;
				}
			}
		}
	}
	/* coz, to zadziala <=> gdy user bedzie mial 4GB ramu i caly sie
		wyczerpie, czyli mozna uznac, ze nigdy */
	printf("out of memory\n");
	return 0;
}

/*
   Gasi bit strony o _numerze_ index. - W.

   NOTKA: funkcja nie jest odporna na zwalnianie wolnych stron :P
*/
void paging_free(uint32 index)
{
	uint8 *pagebmp = (uint8 *)KERNEL_PAGEBMP;
	pagebmp += index >> 3;
	*pagebmp &= ~(1 << (index & 7));
	system_info.used_memory -= 4;
}

/*
   konwertuje wirtualny adres na podstawie pgdir i adresu
   na fizyczny
*/
uint32 get_physical_address(uint32 * PD, uint32 address)
{
	uint32 * PT;

	PT = (uint32)PD[address >> 22] & 0xFFFFF000;
	return ( PT[(address >> 12) & 0x3FF] & 0xFFFFF000 ) | ( address & 0xFFF );
}
