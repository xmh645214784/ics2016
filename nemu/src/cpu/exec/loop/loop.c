#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(loop)
{
	decode_rel_b(eip+1);
	if(cpu.ecx)
	{
		cpu.eip+=(int8_t)op_src->val;
	}
	
	return 2;
}

make_helper(loope)
{
	decode_rel_b(eip+1);
	if(cpu.ecx&&cpu.ZF)
	{
		cpu.eip+=(int8_t)op_src->val;
	}
	
	return 2;
}
make_helper(loopne)
{
	decode_rel_b(eip+1);
	if(cpu.ecx&&cpu.ZF==0)
	{
		cpu.eip+=(int8_t)op_src->val;
	}
	
	return 2;
}
