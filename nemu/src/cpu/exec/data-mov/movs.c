#include "cpu/exec/helper.h"
extern CPU_state cpu;

make_helper(movs_m2m_b)
{

	swaddr_write(cpu.edi,1,swaddr_read(cpu.esi,1,SR_ES),SR_ES);
	int incdec=0;
	if(cpu.DF==0)
		incdec=1;
	else
		incdec=-1;
	cpu.edi+=incdec;
	cpu.esi+=incdec;
	print_asm("movs m8,m8");
	return 1;
}

make_helper(movs_m2m_w)
{
	swaddr_write(cpu.edi,2,swaddr_read(cpu.esi,2,SR_ES),SR_ES);
	int incdec=0;
	if(cpu.DF==0)
		incdec=2;
	else
		incdec=-2;
	cpu.edi+=incdec;
	cpu.esi+=incdec;
	print_asm("movs m16,m16");
	return 1;
}

make_helper(movs_m2m_l)
{
	swaddr_write(cpu.edi,4,swaddr_read(cpu.esi,4,SR_ES),SR_ES);
	int incdec=0;
	if(cpu.DF==0)
		incdec=4;
	else
		incdec=-4;
	cpu.edi+=incdec;
	cpu.esi+=incdec;
	print_asm("movs m32,m32");
	return 1;
}


make_helper_v(movs_m2m)