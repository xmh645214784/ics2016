#include "cpu/exec/template-start.h"
#include "debug.h"

//暂时
#ifndef condition
	#define condition e

#endif

#define instr concat(j,condition)  
	//j+condition


extern CPU_state cpu;

//通用执行函数
static void do_execute() {

//JE
	if(cpu.ZF==1)
	{	
		cpu.eip+=(DATA_TYPE_S)op_src->val;

		#if DATA_BYTE ==2
			cpu.eip=cpu.eip&0x0000FFFF;
		#endif
	}
	print_asm_template1();
}

make_instr_helper(rel)


#include "cpu/exec/template-end.h"
