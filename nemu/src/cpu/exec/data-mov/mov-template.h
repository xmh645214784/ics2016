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



/**
 * The reg field within the ModRM byte specifies which of the special
 * registers in each category is involved. The two bits in the field always 11. 
 * The r/m field specifies the general register involved.
 */


/**
 * we use decode r2rm
 * op_src= cr
 * op_dest=rm
 */
make_helper(mov_r2cr_l) {
	int len=decode_r2rm_l(eip+1);
	//uint32_t modrm = instr_fetch(eip + 1, 1,SR_CS);
	//uint32_t cr_index = (modrm >> 3) & 0x7;
//	printf("modrm reg=%d\n cr_index=%d\n",op_src->reg,cr_index );

#ifdef DEBUG
	//assert((modrm>>6)==3);
	//assert(cr_index==op_src->reg&&cr_index==0);
	assert(len==1);
#endif


/**
 * now only have cr0 cr3
 */
	switch (op_src->reg)
	{
		case 0:	cpu.cr0.val = op_dest->val; break;
		case 3:	
		cpu.cr3.val = op_dest->val; 
		#ifdef TLB
		extern void flush_TLB(); 
		flush_TLB(); 
		#endif
		break;
		default:assert(0);
	}



	print_asm("mov %s, %%cr%d", op_src->str, op_src->reg);

    return len+1;
}

make_helper(mov_cr2r_l) {
	int len=decode_r2rm_l(eip+1);
	//uint32_t modrm = instr_fetch(eip + 1, 1,SR_CS);
	//uint32_t cr_index = (modrm >> 3) & 0x7;
	//assert(cr_index==op_src->reg&&cr_index==0);
/**
 * now only have cr0
 */
	assert(op_src->reg==0);
	OPERAND_W(op_dest,cpu.cr0.val);


	print_asm("mov %%cr%d,%s ",  op_src->reg,op_src->str);

    return len+1;
}


#endif

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4,SR_DS);
	MEM_W(addr, REG(R_EAX),SR_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4,SR_DS);
	REG(R_EAX) = MEM_R(addr,SR_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE==2
make_helper(concat(mov_rm2seg_,SUFFIX))
{
	int len=decode_rm2r_w(eip+1);
	uint32_t modrm = instr_fetch(eip + 1, 1,SR_CS);
	int index = (modrm >> 3) & 0x7;
	print_asm("mov %s, %%%s", op_src->str, segreg_name[index]);
	extern void load_segment(int index,Selector selector);
	Selector selector;
	selector.val=op_src->val;
	load_segment(index,selector);
	return 1+len;
}
#endif
#include "cpu/exec/template-end.h"
