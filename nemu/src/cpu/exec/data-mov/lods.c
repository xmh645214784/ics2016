#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(lods_m_b)
{
	cpu.al=swaddr_read(cpu.esi,1,SR_ES);
	if(cpu.DF==0)
		cpu.esi+=1;
	else
		cpu.esi-=1;
	print_asm("lods m8");
	return 1;
}


make_helper(lods_m_w)
{
	cpu.ax=swaddr_read(cpu.esi,2,SR_ES);
	if(cpu.DF==0)
		cpu.esi+=2;
	else
		cpu.esi-=2;

	print_asm("lods m16");
	return 1;
}


make_helper(lods_m_l)
{
	cpu.eax=swaddr_read(cpu.esi,4,SR_ES);
	if(cpu.DF==0)
		cpu.esi+=4;
	else
		cpu.esi-=4;

	print_asm("lods m32");
	return 1;
}

make_helper_v(lods_m)

