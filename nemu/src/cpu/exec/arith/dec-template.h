#include "cpu/exec/template-start.h"

#define instr dec


static void do_execute () {
	DATA_TYPE_S result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	DATA_TYPE src=1;
	DATA_TYPE des=op_src->val;


 	CPU_AFFECT_ZF(src,des,0)
 	CPU_AFFECT_PF(src,des,0)
 	CPU_AFFECT_SF(src,des,0)
  	CPU_AFFECT_OF(src,des,0)

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
