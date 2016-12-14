#include "cpu/exec/helper.h"
make_helper(cbw_l)
{
	int16_t temp=cpu.ax;
	cpu.eax=(int32_t)temp;
	print_asm("cbwl");
	return 1;
}

make_helper(cbw_w)
{
	int8_t temp=cpu.al;
	cpu.ax=(int16_t)temp;
	print_asm("cbww");
	return 1;
}
make_helper_v(cbw)