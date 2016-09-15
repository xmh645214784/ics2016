#include "cpu/exec/template-start.h"
#include "debug.h"
#define instr push

extern CPU_state cpu;

//通用执行函数
static void do_execute() {

//push 注意可能有错！！
if(DATA_BYTE==2)
{
	cpu.esp-=2;

}
else
{
	cpu.esp-=4;
}
	op_dest->type=OP_TYPE_MEM;
	op_dest->size=DATA_BYTE;
	op_dest->addr=cpu.esp;
	OPERAND_W(op_dest, op_src->val);
	print_asm_template1();
}



#if DATA_BYTE==2||DATA_BYTE==4
	make_instr_helper(r)
	make_instr_helper(m)
#endif

make_instr_helper(i)


#include "cpu/exec/template-end.h"
