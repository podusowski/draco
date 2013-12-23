#ifndef __EXCEPTION__H
#define __EXCEPTION__H

void exception_hardware(uint32 nr, uint32 cr2);
void exception_software(uint32 nr);

#endif
