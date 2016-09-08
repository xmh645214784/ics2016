#include "cpu/exec/template-start.h"

#define instr call

extern CPU_state cpu;
//通用执行函数
static void do_execute() {

#if DATA_BYTE ==4
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+5);
	cpu.eip+=(DATA_TYPE_S)op_src->val;
	print_asm_template1();

#elif DATA_BYTE ==2
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,cpu.ip+3);
	cpu.eip=(cpu.eip+(DATA_TYPE_S)op_src->val)&0x0000FFFF;
	print_asm_template1();

#endif

}
make_instr_helper(rel)

#include "cpu/exec/template-end.h"