#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
/**
 * 320*200=FA00 H
 */

// static PDE vpdir[NR_PDE] align_to_page;						// video page directory
// static PTE vptable[(VMEM_ADDR+SCR_SIZE)/ PAGE_SIZE+1] align_to_page;		// video page tables
static PTE vptable[NR_PTE] align_to_page;		// video page tables
/* Use the function to get the start address of user page directory. */
PDE* get_updir();

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
//BEFORE:	PDE * updir=va_to_pa(get_updir());
//Thanks LWB
//To quote val,we mustnt use va to pa 
	PDE * updir=get_updir();
	PTE *vtable = (PTE *)va_to_pa(vptable);



	/* make 1 PDE invalid */
// 	memset(updir, 0, 1 * sizeof(PDE));
// #ifdef DEBUG
// 	assert(updir->val==0);
// #endif
	/* fill the first PDE */
	updir->val=make_pde(vtable);


	/* fill PTEs */
	uint32_t pframe_addr = VMEM_ADDR;

	/* add vtable to align to 0xa0000*/
	vtable+=160;

	/* fill PTEs straightly */
	for (; pframe_addr <VMEM_ADDR+SCR_SIZE; pframe_addr += PAGE_SIZE) {
		((PTE *)(pa_to_va((void *)vtable)))->val = make_pte(pframe_addr);
		vtable++;
	}

}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

