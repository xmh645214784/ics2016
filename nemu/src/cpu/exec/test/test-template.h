#include "cpu/exec/template-start.h"
#include "debug.h"
#define instr test

extern CPU_state cpu;

bool is_even_number_of_1(uint32_t val);


//通用执行函数
static void do_execute() {
	cpu.CF=0;
	cpu.OF=0;

//ZF
	DATA_TYPE result=op_dest->val&op_src->val;
	if(result==0)
		{
			cpu.ZF=1;
			Log("test中ZF为%d",cpu.ZF);
		}
	else
		cpu.ZF=0;

//SF
#if DATA_BYTE ==1
	if((result&0x10)==1)
		cpu.SF=1;
	else
		cpu.SF=0;

#elif DATA_BYTE ==2
	if((result&0x1000)==1)
		cpu.SF=1;
	else
		cpu.SF=0;
#else
	if((result&0x10000000)==1)
		cpu.SF=1;
	else
		cpu.SF=0;
#endif

//PF
	if(is_even_number_of_1(result))
		cpu.PF=1;
	else
		cpu.PF=0;

	print_asm_template2();
}


make_instr_helper(i2a)


make_instr_helper(r2rm)

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"