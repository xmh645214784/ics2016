#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(leave_w)
{
	cpu.esp=cpu.ebp;
	cpu.bp=swaddr_read(cpu.esp,2);
	cpu.esp+=2;
	print_asm("leavew");
	return 1;
}


make_helper(leave_l)
{
	cpu.esp=cpu.ebp;
	cpu.ebp=swaddr_read(cpu.esp,4);
	cpu.esp+=4;
	print_asm("leavel");
	return 1;
}

make_helper_v(leave)