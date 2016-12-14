#include "cpu/exec/helper.h"
make_helper(bt_r2rm_w)
{
	int len=decode_r2rm_w(eip+1);
	cpu.CF=(op_src->val>>op_dest->val)&1;
	print_asm("bt %s, %s",op_src->str,op_dest->str);
	return 1+len;
}

make_helper(bt_r2rm_l)
{
	int len=decode_r2rm_l(eip+1);
	cpu.CF=(op_src->val>>op_dest->val)&1;
	print_asm("bt %s, %s",op_src->str,op_dest->str);
	return 1+len;
}

make_helper_v(bt_r2rm)