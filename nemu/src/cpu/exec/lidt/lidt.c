#include "cpu/exec/helper.h"


static inline void do_execute() { // move gpr to cr
#ifdef DEBUG
	extern Operands ops_decoded;
	Assert(ops_decoded.is_operand_size_16==0,"lgdt 16bit");
#endif
	print_asm("lidt fword %s", op_src->str);
	cpu.idtr = lnaddr_read(op_src->addr + 2, 4);
	cpu.idtrlimit = lnaddr_read(op_src->addr, 2);

}

make_helper(lidt_rm2r_l) {
    return idex(eip, decode_rm2r_l, do_execute);
}
