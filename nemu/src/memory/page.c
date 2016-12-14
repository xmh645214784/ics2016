#include "cpu/reg.h"
#include "common.h"
#include "../../../lib-common/x86-inc/mmu.h"
#include "memory/memory.h"


	#define DIR(addr) ((addr>>22)&0x3ff)
	#define PAGE(addr) ((addr>>12)&0x3ff)
	#define OFFSET(addr) (addr &0xfff)

/**
 * [find_hwaddr_due_lnaddr description]
 * return real hwaddr
 * @param  addr [description]
 * @return      [description]
 */
static hwaddr_t find_hwaddr_due_lnaddr(lnaddr_t addr)
{
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




hwaddr_t page_translate(lnaddr_t addr)
{
#ifdef TLB
hwaddr_t read_TLB(lnaddr_t addr);
	return read_TLB(addr)+OFFSET(addr);
#else
	return find_hwaddr_due_lnaddr(addr);
#endif

}

#ifdef TLB
/////////////////////////////////////////////////////////////////////////////
/**
 *  TLB
 */
typedef struct 
{
	uint32_t ln_tag:20;
	uint32_t hw_result:20;
	uint32_t valid:1;

}TLBSlot;

#define NR_TLBSLOT 60
TLBSlot tlb [NR_TLBSLOT];

void init_TLB()
{
	int i=0;
	for(;i<NR_TLBSLOT;i++)
		tlb[i].valid=0;
}

void flush_TLB()
{
	init_TLB();
}




/**
 * [allocate_TLBSlot description]
 * 	allocate one TLBSlot due to the addr and return hw:20
 * @param  addr [description]
 * @return      [return hw:20]
 */
static hwaddr_t allocate_TLBSlot(lnaddr_t addr)
{
	int i=0;
	for(;i<NR_TLBSLOT;i++)
		if(tlb[i].valid==0)
		{
			/**
			 * have unused TLBSlot
			 */
			tlb[i].valid=1;
			tlb[i].ln_tag=((DIR(addr)<<10)|PAGE(addr));
			tlb[i].hw_result=find_hwaddr_due_lnaddr(addr)>>12;	
			return tlb[i].hw_result;																														 
		}

	/**
	 * dont have unused TLBSlot
	 * "randomly" fuck on tlb[0]
	 */

	tlb[0].ln_tag=((DIR(addr)<<10)|PAGE(addr));
	tlb[0].hw_result=find_hwaddr_due_lnaddr(addr)>>12;	
	return tlb[0].hw_result;	
}



/**
 * [read_TLB description]
 * @param  addr [description]
 * @return      [return real hw_addr.It has been  <<12]
 */
hwaddr_t read_TLB(lnaddr_t addr)
{
	int i=0;
	for(;i<NR_TLBSLOT;i++)
		if(tlb[i].valid==1&&tlb[i].ln_tag==((DIR(addr)<<10)|PAGE(addr)))
		{
			/**
			 * HIT TLB
			 */
			return tlb[i].hw_result<<12;
		}

	/**
	 * MISS TLB
	 */
	return allocate_TLBSlot(addr)<<12;
}
#endif

#undef DIR
#undef PAGE
#undef OFFSET


