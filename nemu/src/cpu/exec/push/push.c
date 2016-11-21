#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper(push_ib_w)
{
	decode_i_b(eip+1);
	int8_t a=op_src->val;
	int b=a;
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,b,SR_SS);
	print_asm("push %s",op_src->str);
	return 2;
}
make_helper(push_ib_l)
{
	decode_i_b(eip+1);
	int8_t a=op_src->val;
	int b=a;
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,b,SR_SS);
	print_asm("push %s",op_src->str);
	return 2;
}

make_helper_v(push_ib)
make_helper_v(push_r)
make_helper_v(push_m)
make_helper_v(push_i)
