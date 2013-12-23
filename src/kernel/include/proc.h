#ifndef __PROC__H
#define __PROC__H

void proc_init();
uint32 proc_add(char * name, uint8 class, uint32 address, uint32 image_size, uint32 e_entry);
uint32 proc_kill(uint32 pid);
uint32 proc_process_env(uint32 image_size);
uint32 proc_setheapsize(uint32 pid, uint32 size);
uint32 proc_createsharedframe(uint32 pid);

#endif 
