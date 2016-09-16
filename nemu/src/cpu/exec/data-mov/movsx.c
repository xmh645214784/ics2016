#include "cpu/exec/helper.h"

make_helper(movzx_rmb2r_w)
{
	int len=decode_rm2r_w(eip);
	uint8_t a=op_src->val;
	uint16_t b=a;//fuhaokuozhang
	reg_w(op_dest->reg)=b;
	//write_operand_w(op_des,(uint16_t)a);
	return 2+len;
}

make_helper(movzx_rmb2r_l)
{
	int len=decode_rm2r_w(eip);
	uint8_t a=op_src->val;
	uint32_t b=a;
	//write_operand_l(op_des,(uint32_t)a);
	reg_l(op_dest->reg)=b;
	return 2+len;
}

make_helper(movzx_rmw2r_l)
{
	int len=decode_rm2r_l(eip);
	uint16_t a=op_src->val;
	uint32_t b=a;
	//write_operand_l(op_des,(uint32_t)a);
	reg_l(op_dest->reg)=b;
	return 2+len;
}

make_helper_v(movzx_rmb2r)
