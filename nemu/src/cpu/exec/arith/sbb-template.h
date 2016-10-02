#include "cpu/exec/template-start.h"
#include "debug.h"
#define instr sbb

//通用执行函数
static void do_execute() {
	DATA_TYPE src=op_src->val+cpu.CF;
	DATA_TYPE des=op_dest->val;
	DATA_TYPE result=des-src;
	OPERAND_W(op_dest,result);
//Log("目的操作数类型%d reg标号为%d 值为%d,",op_dest->type,op_dest->reg,op_dest->val);
//Log("cmp %d-%d",src,des);

 	CPU_AFFECT_CF(src,des,0)
 	CPU_AFFECT_ZF(src,des,0)
 	CPU_AFFECT_PF(src,des,0)
 	CPU_AFFECT_SF(src,des,0)
  	CPU_AFFECT_OF(src,des,0)
  	
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE==2||DATA_BYTE==4
	make_instr_helper(si2rm)
#endif


#include "cpu/exec/template-end.h"