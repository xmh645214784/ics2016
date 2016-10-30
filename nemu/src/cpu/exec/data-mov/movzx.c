#include "cpu/exec/helper.h"


//BUG
make_helper(movzx_rmb2r_w)
{
	int len=decode_rm2r_b(eip+1);
	uint8_t a=op_src->val;
	uint16_t b=a;//fuhaokuozhang
	reg_w(op_dest->reg)=b;
	//write_operand_w(op_des,(uint16_t)a);
	print_asm("movzx %s,%s",op_src->str,op_dest->str);
	return 1+len;
}

make_helper(movzx_rmb2r_l)
{
	int len=decode_rm2r_b(eip+1);
	uint8_t a=op_src->val;
	uint32_t b=a;
	//write_operand_l(op_des,(uint32_t)a);
	reg_l(op_dest->reg)=b;
	print_asm("movzx %s,%s",op_src->str,op_dest->str);
	return 1+len;
}

make_helper(movzx_rmw2r_l)
{
	int len=decode_rm2r_w(eip+1);
	uint16_t a=op_src->val;
	uint32_t b=a;
	//write_operand_l(op_des,(uint32_t)a);
	reg_l(op_dest->reg)=b;
	print_asm("movzx %s,%s",op_src->str,op_dest->str);
	return 1+len;
}

make_helper_v(movzx_rmb2r)
