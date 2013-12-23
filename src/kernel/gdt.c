#include "draco.h"

#define MODULE_NAME "gdt"

/* 
   Nalezy tutaj zapamietac, ze rejestry segmentowe zaaladowane
   sa wg GDT bootloadera i nie ma sensu ich zmieniac, w tej
   chwili mozemy modyfikowac (dodawac/usuwac) deskryptory.
   Nie czyszcze tego miejsca bo jesli nie zaladujemy blednego
   selektora do rejestrow to nic zlego nie powinno sie stac - W.
*/
void init_gdt()
{
	sys_desc *desc = (sys_desc*)KERNEL_GDT;

	/*
      NULLa nie da sie ustawic z setup_seg, wiec trzeba to
      zrobic tutaj (defaultowo ustawia G) 
	*/
	desc[0].limit 		= 0;
	desc[0].base_0_15	= 0;
	desc[0].base_16_23	= 0;
	desc[0].dpl_type	= 0;
	desc[0].gav_lim		= 0;
	desc[0].base_24_31	= 0;

	setup_seg(1, 0, 0x000FFFFF, SEGMENT_EXE | SEGMENT_RING0);
	setup_seg(2, 0, 0x000FFFFF, SEGMENT_RW | SEGMENT_RING0);

	// dwa segmenty dla aplikacji
	setup_seg(3, 0, 0x000FFFFF, SEGMENT_EXE | SEGMENT_RING3);
	setup_seg(4, 0, 0x000FFFFF, SEGMENT_RW | SEGMENT_RING3);

	load_gdt();
}

/* 
   Na limit mamy 20 bitow, wiec trzeba to uwzglednic przy wywolywaniu
   funkcji, starsze bity zostana usuniete (granularity jest zapalony)
   wiec adres jest mnozony przez 4KB (lub jak kto woli podajemy ilosc
   stron w segmencie). - W.
*/
void setup_seg(uint8 index, uint32 base, uint32 limit, uint8 type)
{
	sys_desc *desc = (sys_desc*)KERNEL_GDT;

/*
  ; CODE, BASE:0, LIMIT: 4GB, DPL:0
  dw 0xFFFF    ; low word of limit
  dw 0         ; low word of base
  db 0         ; low byte of high word of base
  db 10011010b ; code exec-read
  db 11001111b ; flags and 4 bits of limit
  db 0         ; highest byte of base
*/

	desc[index].limit      = limit & 0xFFFF;
	desc[index].base_0_15  = base << 16;
	desc[index].base_16_23 = (base & 0xFF0000) >> 16;
	desc[index].dpl_type   = type;
	desc[index].gav_lim    = ((limit & 0xF0000) >> 16) | 0xC0;
	desc[index].base_24_31 = 0;
}


