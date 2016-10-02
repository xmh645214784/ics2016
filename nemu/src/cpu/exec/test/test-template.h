#include "cpu/exec/template-start.h"
#include "debug.h"
#define instr test

//通用执行函数
static void do_execute() {
	cpu.CF=0;
	cpu.OF=0;



	DATA_TYPE result=op_dest->val&op_src->val;

	CPU_AFFECT_ZF(result,0,1)
	CPU_AFFECT_SF(result,0,1)
	CPU_AFFECT_PF(result,0,1)


	print_asm_template2();
}


make_instr_helper(i2a)


make_instr_helper(r2rm)

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"