#include "cpu/exec/helper.h"

make_helper(jmp_rel_b)
{
	decode_i_b(eip+1);
	cpu.eip+=(int8_t)op_src->val;
	print_asm("jmp %s",op_src->str);
	return 2;
}

make_helper(jmp_rel_w)
{
	decode_i_w(eip+1);
	cpu.eip+=(int16_t)op_src->val;
	cpu.eip=cpu.eip&0xFFFF;
	print_asm("jmp %s",op_src->str);
	return 3;
}

make_helper(jmp_rel_l)
{
	decode_i_l(eip+1);
	cpu.eip+=(int32_t)op_src->val;
	print_asm("jmp %s",op_src->str);
	return 5;
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

