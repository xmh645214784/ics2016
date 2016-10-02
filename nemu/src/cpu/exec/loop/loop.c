#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(loop_l)
{
	decode_rel_b(eip+1);
	if(cpu.ecx)
	{
		cpu.eip+=(int8_t)op_src->val;
	}
	
	return 2;
}

make_helper(loope_l)
{
	decode_rel_b(eip+1);
	if(cpu.ecx&&cpu.ZF)
	{
		cpu.eip+=(int8_t)op_src->val;
	}
	
	return 2;
}


make_helper(loopne_l)
{
	decode_rel_b(eip+1);
	if(cpu.ecx&&cpu.ZF==0)
	{
		cpu.eip+=(int8_t)op_src->val;
	}
	
	return 2;
}




make_helper(loop_w)
{
	decode_rel_b(eip+1);
	if(cpu.ecx)
	{
		cpu.ip+=(int8_t)op_src->val;
	}
	
	return 2;
}

make_helper(loope_w)
{
	decode_rel_b(eip+1);
	if(cpu.ecx&&cpu.ZF)
	{
		cpu.ip+=(int8_t)op_src->val;
	}
	
	return 2;
}
make_helper(loopne_w)
{
	decode_rel_b(eip+1);
	if(cpu.ecx&&cpu.ZF==0)
	{
		cpu.ip+=(int8_t)op_src->val;
	}
	
	return 2;
}

make_helper_v(loop)
make_helper_v(loope)
make_helper_v(loopne)