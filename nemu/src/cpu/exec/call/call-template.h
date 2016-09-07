#include "cpu/exec/template-start.h"

#define instr call

extern CPU_state cpu;
//通用执行函数
static void do_execute() {

#if SUFFIX==l
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+5);
	cpu.eip+=op_src->simm;
	print_asm_template1();
#elif SUFFIX==w
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,cpu.ip+3);
	cpu.eip=(eip+op_src->simm)&0x0000FFFF;
	print_asm_template1();

#endif

}
make_instr_helper(rel)

#include "cpu/exec/template-end.h"