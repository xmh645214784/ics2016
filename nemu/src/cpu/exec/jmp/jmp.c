#include "cpu/exec/helper.h"

make_helper(jmp_rel_b)
{
	int len=decode_i_b(eip+1);
	cpu.eip+=(int8_t)op_src->val;
	print_asm("jmp %s",op_src->str);
	return 1+len;
}

make_helper(jmp_rel_w)
{
	int len=decode_i_w(eip+1);
	cpu.eip+=(int16_t)op_src->val;
	cpu.eip=cpu.eip&0xFFFF;
	print_asm("jmp %s",op_src->str);
	return 1+len;
}

make_helper(jmp_rel_l)
{
	int len=decode_i_l(eip+1);
	cpu.eip+=(int32_t)op_src->val;
	print_asm("jmp %s",op_src->str);
	return 1+len;
}


make_helper(jmp_rm_w)
{
	decode_rm2r_w(eip+1);
	cpu.eip=(int16_t)op_src->val&0x0000FFFF;
	print_asm("jmp %s",op_src->str);
	return 0;
}

make_helper(jmp_rm_l)
{
	decode_rm2r_l(eip+1);
	cpu.eip=(int32_t)op_src->val;
	print_asm("jmp %s",op_src->str);
	return 0;
}


/* for instruction encoding overloading */
make_helper_v(jmp_rel)
make_helper_v(jmp_rm)
#include "cpu/exec/helper.h"

