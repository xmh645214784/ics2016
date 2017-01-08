#include "cpu/exec/template-start.h"

#define instr inc
static void do_execute () {
	
	DATA_TYPE src=op_src->val;
	DATA_TYPE des=1;

	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);


 /*	CPU_AFFECT_CF(src,des,1)*/
 	CPU_AFFECT_ZF(src,des,1)
 	CPU_AFFECT_PF(src,des,1)
 	CPU_AFFECT_SF(src,des,1)
  	CPU_AFFECT_OF(src,des,1)
	/* TODO: Update EFLAGS. */
	//panic("please implement me");

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
