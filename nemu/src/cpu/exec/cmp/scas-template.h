#include "cpu/exec/template-start.h"

#define instr scas

//通用执行函数
static void do_execute() {
	DATA_TYPE_S src=-MEM_R(cpu.edi);
	DATA_TYPE_S des=REG(0);
	CPU_AFFECT_CF(src,des,0)
 	CPU_AFFECT_ZF(src,des)
 	CPU_AFFECT_PF(src,des)
 	CPU_AFFECT_SF(src,des)
  	CPU_AFFECT_OF(src,des)
  	if(cpu.DF==0)
  		cpu.edi+=DATA_BYTE;
  	else
  		cpu.edi-=DATA_BYTE;
	print_asm("scas");
}
make_instr_helper(none)

#include "cpu/exec/template-end.h"
