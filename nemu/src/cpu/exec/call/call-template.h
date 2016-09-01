#ifndef _CALL_TEMPLATE_H
#define _CALL_TEMPLATE_H
#include "cpu/exec/template-start.h"

#define instr call

extern CPU_state cpu;
//通用执行函数
static void do_execute() {

#if SUFFIX==l
	swaddr_write(cpu.esp,1,cpu.eip);
	cpu.esp-=4;
	cpu.eip+=op_dest->imm;

#else 
	#error NOT call_32
#endif



	print_asm_template1();
}

make_instr_helper(rel)

#include "cpu/exec/template-end.h"
#endif