#include "cpu/exec/helper.h"

make_helper(movsx_rmb2r_w)
{
	int len=decode_rm2r_w(eip);
	int8_t a=op_src->val;
	int16_t b=a;//fuhaokuozhang
	reg_w(op_dest->reg)=b;
	//write_operand_w(op_des,(int16_t)a);
	return 2+len;
}

make_helper(movsx_rmb2r_l)
{
	int len=decode_rm2r_w(eip);
	int8_t a=op_src->val;
	int32_t b=a;
	//write_operand_l(op_des,(int32_t)a);
	reg_l(op_dest->reg)=b;
	return 2+len;
}

make_helper(movsx_rmw2r_l)
{
	int len=decode_rm2r_l(eip);
	int16_t a=op_src->val;
	int32_t b=a;
	//write_operand_l(op_des,(int32_t)a);
	reg_l(op_dest->reg)=b;
	return 2+len;
}

make_helper_v(movsx_rmb2r)
