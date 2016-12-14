#include "cpu/exec/helper.h"

make_helper(lgdt_rm2r_l)
{
	int len=decode_rm2r_l(eip+1);
#ifdef DEBUG
	Assert(ops_decoded.is_operand_size_16==0,"lgdt 16bit");
#endif
	print_asm("lgdt fword %s", op_src->str);
	cpu.gdtr = lnaddr_read(op_src->addr + 2, 4);
	cpu.gdtrlimit = lnaddr_read(op_src->addr, 2);
	return 1+len;
}
