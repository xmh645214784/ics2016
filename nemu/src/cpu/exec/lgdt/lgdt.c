#include "cpu/exec/helper.h"


static inline void do_execute_rm2r() { // move gpr to cr
#ifdef DEBUG
	extern Operands ops_decoded;
	Assert(ops_decoded.is_operand_size_16==0,"lgdt 16bit");
#endif
	print_asm("lgdt fword %s", op_src->str);
	cpu.gdtr = lnaddr_read(op_src->addr + 2, 4);
	cpu.gdtrlimit = lnaddr_read(op_src->addr, 2);

}

make_helper(lgdt_rm2r_l) {
    return idex(eip, decode_rm2r_l, do_execute_rm2r);
}
