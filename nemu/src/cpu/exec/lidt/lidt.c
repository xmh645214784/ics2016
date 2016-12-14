#include "cpu/exec/helper.h"


make_helper(lidt_rm2r_l)
{
	int len=decode_rm2r_l(eip+1);
#ifdef DEBUG
	Assert(ops_decoded.is_operand_size_16==0,"lidt 16bit");
#endif
	print_asm("lidt fword %s", op_src->str);
	cpu.idtr = lnaddr_read(op_src->addr + 2, 4);
	cpu.idtrlimit = lnaddr_read(op_src->addr, 2);
	return 1+len;
}