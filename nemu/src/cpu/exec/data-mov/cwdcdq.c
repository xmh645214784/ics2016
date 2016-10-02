#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(cwdcdq_w)
{
	if((int16_t)cpu.ax<0)
		cpu.dx=0xFFFF;
	else
		cpu.dx=0;
	print_asm("cwdw");
	return 1;
}


make_helper(cwdcdq_l)
{
	if((int)cpu.eax<0)
		cpu.edx=0xFFFFFFFF;
	else
		cpu.edx=0;
	print_asm("cwdl");
	return 1;
}

make_helper_v(cwdcdq)