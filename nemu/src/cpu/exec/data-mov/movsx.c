#include "cpu/exec/helper.h"

//BUG
make_helper(movsx_rmb2r_w)
{
	int len=decode_rm2r_b(eip+1);
	int8_t a=op_src->val;
	int16_t b=a;//fuhaokuozhang
	reg_w(op_dest->reg)=b;
	//write_operand_w(op_des,(int16_t)a);
	print_asm("movsbw %s,%s(jia)",op_src->str,op_dest->str);
	return 1+len;
}

make_helper(movsx_rmb2r_l)
{
	int len=decode_rm2r_b(eip+1);
	int8_t a=op_src->val;
	int32_t b=a;
	//write_operand_l(op_des,(int32_t)a);
	reg_l(op_dest->reg)=b;
	print_asm("movsbl %s,%s(jia)",op_src->str,op_dest->str);
	return 1+len;
}

make_helper(movsx_rmw2r_l)
{
	int len=decode_rm2r_w(eip+1);
	int16_t a=op_src->val;
	int32_t b=a;
	//write_operand_l(op_des,(int32_t)a);
	reg_l(op_dest->reg)=b;
	print_asm("movswl %s,%s(jia)",op_src->str,op_dest->str);
	return 1+len;
}

make_helper_v(movsx_rmb2r)
