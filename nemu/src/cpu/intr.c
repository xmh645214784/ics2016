#include <setjmp.h>
#include "common.h"
#include "cpu/reg.h"
extern jmp_buf jbuf;
#include "../../../lib-common/x86-inc/mmu.h"
#include "memory/memory.h"

void raise_intr(uint8_t NO) {
	/* TODO:Trigger an interrupt/exception with ``NO''
	 * That is, use ``NO'' to index the IDT.
	 */
	
	//EFLAGS
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.EFLAGS,SR_SS);
	

	/**
	 * CS  BUG??????
	 */
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.cs.selector.val,SR_SS);
	
	
	//EIP
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip,SR_SS);



    //GATE  
    GateDesc desc;
	uint32_t desc_data[2];
	desc_data[0] = lnaddr_read(cpu.idtr + NO * 8, 4); // little endian
	desc_data[1] = lnaddr_read(cpu.idtr + NO * 8 + 4, 4);
	memcpy(&desc, desc_data, 8);
#ifdef DEBUG
	assert(desc.present==1);
#endif

	
	/* Jump back to cpu_exec() */	 
	longjmp(jbuf, 1);
}