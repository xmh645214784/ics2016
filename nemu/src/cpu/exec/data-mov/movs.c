#include "cpu/exec/helper.h"
extern CPU_state cpu;

make_helper(movs_m2m_b)
{

	swaddr_write(cpu.edi,1,swaddr_read(cpu.esi,1));
	print_asm("movs m8,m8");
	return 1;
}

make_helper(movs_m2m_w)
{
	swaddr_write(cpu.edi,2,swaddr_read(cpu.esi,2));
	print_asm("movs m16,m16");
	return 1;
}

make_helper(movs_m2m_l)
{
	swaddr_write(cpu.edi,4,swaddr_read(cpu.esi,4));
	print_asm("movs m32,m32");
	return 1;
}


make_helper_v(movs_m2m)