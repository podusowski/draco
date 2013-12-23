#ifndef __GDT__H
#define __GDT__H 

#define SEGMENT_R	0x00 //0000 data read-only
#define SEGMENT_RW	0x02 //0010 data read/write
#define SEGMENT_EXE 0x0A //1010 code exec/read
#define SEGMENT_RING0 0x90 
#define SEGMENT_RING3 0xF0

void init_gdt();
void setup_seg(uint8 index, uint32 base, uint32 limit, uint8 type);
extern void load_gdt();

#endif
