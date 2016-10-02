#include "cpu/exec/template-start.h"

#define instr cmp

//通用执行函数
static void do_execute() {
	//BUG?
	DATA_TYPE_S src=-(DATA_TYPE_S)op_src->val;
	DATA_TYPE_S des=(DATA_TYPE_S)op_dest->val;
	Log("des-src=%x-%x",des,-src);
	Log("DATA_BYTE=%d",DATA_BYTE);
	CPU_AFFECT_CF(src,des,0)
 	CPU_AFFECT_ZF(src,des)
 	CPU_AFFECT_PF(src,des)
 	CPU_AFFECT_SF(src,des)
  	CPU_AFFECT_OF(src,des,0)

	print_asm_template2();
}
make_instr_helper(i2a)
make_instr_helper(i2rm)

//sign extend 
#if DATA_BYTE==2||DATA_BYTE==4
	make_instr_helper(si2rm)
#endif


make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
