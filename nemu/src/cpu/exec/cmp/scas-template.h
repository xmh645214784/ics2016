#include "cpu/exec/template-start.h"

#define instr scas

//通用执行函数
static void do_execute() {
	DATA_TYPE src=MEM_R(cpu.edi,SR_ES);
	DATA_TYPE des=REG(R_EAX);
	CPU_AFFECT_CF(src,des,0)
 	CPU_AFFECT_ZF(src,des,0)
 	CPU_AFFECT_PF(src,des,0)
 	CPU_AFFECT_SF(src,des,0)
  	CPU_AFFECT_OF(src,des,0)
  	if(cpu.DF==0)
  		cpu.edi+=DATA_BYTE;
  	else
  		cpu.edi-=DATA_BYTE;
	print_asm("scas");
}
make_instr_helper(none)

#include "cpu/exec/template-end.h"
