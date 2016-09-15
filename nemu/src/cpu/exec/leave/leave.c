#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(leave_w)
{
	cpu.esp=cpu.ebp;
	cpu.bp=swaddr_read(cpu.esp,2);
	cpu.esp+=2;
	return 1;
}


make_helper(leave_l)
{
	cpu.esp=cpu.ebp;
	cpu.ebp=swaddr_read(cpu.esp,4);
	cpu.esp+=4;
	return 1;
}

make_helper_v(leave)