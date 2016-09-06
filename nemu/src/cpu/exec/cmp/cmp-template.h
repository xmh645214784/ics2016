#include "cpu/exec/template-start.h"

#define instr cmp

extern CPU_state cpu;
bool is_even_number_of_1(uint32_t val);
//通用执行函数
static void do_execute() {
	int result=op_src->simm-op_dest->simm;
//OF
	if(op_src->simm>0&&op_dest->simm<0&&result<0)
		cpu.OF=1;
	else if(op_src->simm<0&&op_dest->simm>0&&result>0)
		cpu.OF=1;
	else
		cpu.OF=0;

//SF
	if(result>=0)
		cpu.SF=0;
	else
		cpu.SF=1;
//ZF
	if(result==0)
		cpu.ZF=1;
	else
		cpu.ZF=0;




//PF
	if(is_even_number_of_1(result))
		cpu.PF=1;
	else
		cpu.PF=0;

//CF
	
}



#include "cpu/exec/template-end.h"
