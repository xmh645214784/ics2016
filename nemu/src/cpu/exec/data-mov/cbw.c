#include "cpu/exec/helper.h"
make_helper(cbw_l)
{
	int16_t temp=cpu.ax;
	cpu.eax=temp;
	print_asm("cbwl");
	return 1;
}

make_helper(cbw_w)
{
	int8_t temp=cpu.al;
	cpu.ax=temp;
	print_asm("cbww");
	return 1;
}
make_helper_v(cbw)