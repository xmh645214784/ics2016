#include "cpu/exec/template-start.h"
#include "debug.h"
#define instr sbb

extern CPU_state cpu;
bool is_even_number_of_1(uint32_t val);
//通用执行函数
static void do_execute() {



	
	DATA_TYPE_S src=(DATA_TYPE_S)op_src->val;
	DATA_TYPE_S des=(DATA_TYPE_S)op_dest->val;
	DATA_TYPE_S result=des-(src+cpu.CF);
	OPERAND_W(op_dest,result);
//Log("目的操作数类型%d reg标号为%d 值为%d,",op_dest->type,op_dest->reg,op_dest->val);
//Log("cmp %d-%d",src,des);

//OF
	if(src>0&&des<0&&result<0)
		cpu.OF=1;
	else if(src<0&&des>0&&result>0)
		cpu.OF=1;
	else
		cpu.OF=0;

//SF
	if(result>=0)
		cpu.SF=0;
	else
		cpu.SF=1;
//ZF
	if(result==0)
		cpu.ZF=1;
	else
		cpu.ZF=0;




//PF
	if(is_even_number_of_1(result))
		cpu.PF=1;
	else
		cpu.PF=0;

//CF
	//可能有BUG
	if(result<0)
		cpu.CF=1;
	else
		cpu.CF=0;	
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