#include "cpu/reg.h"
#include "common.h"
#include "../../../lib-common/x86-inc/mmu.h"
#include "memory/memory.h"
hwaddr_t page_translate(lnaddr_t addr)
{
	#define DIR(addr) ((addr>>22)&0x3ff)
	#define PAGE(addr) ((addr>>12)&0x3ff)
	#define OFFSET(addr) (addr &0xfff)

	uint32_t dir=DIR(addr);
	uint32_t page=PAGE(addr);
	uint32_t offset=OFFSET(addr);

	/*it's real 32bit  ;not 20 bit*/
	uint32_t page_directory_base=cpu.cr3.page_directory_base<<12;

	/*find the page dir entry*/	
	PDE pde;
	pde.val=hwaddr_read(page_directory_base+4*dir,4);
#ifdef DEBUG
	assert(pde.present);
#endif

	/*find the page table entry*/	
	PTE pte;
	pte.val=hwaddr_read((pde.page_frame<<12)+4*page,4);

#ifdef DEBUG
	assert(pte.present);
#endif
	return (pte.page_frame<<12)|offset;

}

#undef DIR
#undef PAGE
#undef OFFSET