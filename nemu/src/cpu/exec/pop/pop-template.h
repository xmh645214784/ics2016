#include "cpu/exec/template-start.h"

#define instr pop

extern CPU_state cpu;

//通用执行函数
static void do_execute() {
	OPERAND_W(op_src,swaddr_read(cpu.esp,DATA_BYTE));

	cpu.esp+=DATA_BYTE;
	print_asm_template1();
}

make_instr_helper(m)
make_instr_helper(r)

#include "cpu/exec/template-end.h"
