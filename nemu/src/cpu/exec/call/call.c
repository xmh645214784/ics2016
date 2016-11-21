#include "cpu/exec/helper.h"

make_helper(call_rel_w)
{
	decode_i_w(eip+1);
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,cpu.ip+3,SR_SS);
	cpu.eip=(cpu.eip+(int16_t)op_src->val)&0x0000FFFF;
	print_asm("call "str(op_src->str));
	return 3;
}

make_helper(call_rel_l)
{
	decode_i_l(eip+1);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+5,SR_SS);
	cpu.eip+=(int)op_src->val;
	print_asm("call %s",op_src->str);
	return 5;
}

make_helper(call_rm_w)
{
	int len=decode_rm2r_w(eip+1);
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,cpu.ip+len+1,SR_SS);
	cpu.eip=op_src->val&0x0000FFFFF;
	print_asm("call %s",op_src->str);
	return 0;
}


make_helper(call_rm_l)
{
	int len=decode_rm2r_l(eip+1);
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+len+1,SR_SS);
	cpu.eip=op_src->val;
	print_asm("call %s",op_src->str);
	return 0;
}

make_helper_v(call_rel)
make_helper_v(call_rm)
