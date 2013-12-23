#ifndef __PAGING__H
#define __PAGING__H

void 		paging_init();
void		paging_map(const void *physical_address, const void *virtual_address);
uint32		paging_new();
void		paging_free(uint32 index);
uint32 		get_physical_address(uint32 * PD, uint32 address);

#endif
 
