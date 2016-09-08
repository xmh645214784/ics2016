#include "cpu/exec/template-start.h"
#include "debug.h"
#define instr push

extern CPU_state cpu;

//通用执行函数
static void do_execute() {

//push 注意可能有错！！
#if SUFFIX == w
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,op_src->val);
	print_asm_template1();
#else
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,op_src->val);
	print_asm_template1();
#endif

}
make_instr_helper(r)


#include "cpu/exec/template-end.h"
