#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	//Log("des%x %s src:%x",op_dest->val,REG_NAME(op_dest->reg),op_src->val);
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
 	cpu.OF=0;
 	cpu.CF=0;
 	CPU_AFFECT_ZF(result,0,1)
 	CPU_AFFECT_PF(result,0,1)
 	CPU_AFFECT_SF(result,0,1)
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
