#include "cpu/exec/template-start.h"

#define instr mov

//通用执行函数
static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE==4

make_helper(mov_r2cr_l) {
	int len=decode_r2rm_l(eip+1);
	uint32_t modrm = instr_fetch(cpu.eip + 1, 1);
	int cr_index = (modrm >> 3) & 0x7;

	assert(cr_index==op_dest->reg&&cr_index==0);
/**
 * now only have cr0
 */
	cpu.cr0.val = op_src->val;


	print_asm("mov %s, %%cr%d", op_src->str, cr_index);

    return len+1;
}

make_helper(mov_cr2r_l) {
	int len=decode_r2rm_l(eip+1);
	uint32_t modrm = instr_fetch(eip + 1, 1);
	int cr_index = (modrm >> 3) & 0x7;
	assert(cr_index==op_dest->reg&&cr_index==0);
/**
 * now only have cr0
 */
	OPERAND_W(op_src,cpu.cr0.val);


	print_asm("mov %%cr%d,%s ",  cr_index,op_src->str);

    return len+1;
}


#endif

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
