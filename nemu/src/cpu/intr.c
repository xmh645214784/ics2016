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
	
	/**
	*   1. 依次将EFLAGS, CS, EIP寄存器的值压入堆栈
	*	2. 从IDTR中读出IDT的首地址
	*	3. 根据异常(中断)号在IDT中进行索引, 找到一个门描述符
	*	4. 把门描述符中的selector域装入CS寄存器
	*	5. 根据CS寄存器中的段选择符, 在GDT或LDT中进行索引, 找到一个段描述符, 并把这个段
	*	的一些信息加载到CS寄存器的描述符cache中
	*	6. 在段描述符中读出段的基地址, 和门描述符中的offset域相加, 得出入口地址
	*	7. 跳转到入口地址
	*/
	//EFLAGS
	//
#ifdef DEBUG
	// Log("intr=%d",NO);
#endif
	
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
 	//get gate info
    GateDesc desc;
	uint32_t temp1[2];
	temp1[0] = lnaddr_read(cpu.idtr + NO * 8, 4); // little endian
	temp1[1] = lnaddr_read(cpu.idtr + NO * 8 + 4, 4);
	memcpy(&desc, temp1, 8);
#ifdef DEBUG
	assert(desc.present==1);
#endif

	//get seg selector in gate
	Selector temp2;
	temp2.val=desc.segment;
extern void load_segment(int index,Selector selector);
	load_segment(SR_CS,temp2);//load CS

	cpu.eip=desc.offset_15_0|(desc.offset_31_16<<16);
	

	/* Jump back to cpu_exec() */	 
	longjmp(jbuf, 1);
}