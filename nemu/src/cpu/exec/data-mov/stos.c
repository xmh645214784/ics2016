#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(stos_m_b)
{
	swaddr_write(cpu.edi,1,cpu.al);
	if(cpu.DF==0)
		cpu.edi+=1;
	else
		cpu.edi-=1;

	print_asm("stos m8");
	return 1;
}


make_helper(stos_m_w)
{
	swaddr_write(cpu.edi,2,cpu.ax);
	if(cpu.DF==0)
		cpu.edi+=2;
	else
		cpu.edi-=2;

	print_asm("stos m16");
	return 1;
}


make_helper(stos_m_l)
{
	swaddr_write(cpu.edi,4,cpu.eax);
	if(cpu.DF==0)
		cpu.edi+=4;
	else
		cpu.edi-=4;

	print_asm("stos m32");
	return 1;
}

make_helper_v(stos_m)

