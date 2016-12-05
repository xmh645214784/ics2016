#include "cpu/exec/helper.h"
make_helper(int_i_b)
{
	int len=decode_i_b(eip+1);


	void raise_intr(uint8_t NO);
	

#ifdef DEBUG
	assert((int)op_src->val>0);
#endif	
	raise_intr(op_src->val);
	print_asm("int 0x%02x",op_src->val);
	return 1+len;
}