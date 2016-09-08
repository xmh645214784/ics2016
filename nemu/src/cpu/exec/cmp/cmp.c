#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmp-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */



make_helper(cmp_i82rm_w)
{
	panic("进去了,译码错误");
	decode_rm_imm_w(eip);
	int8_t dest=op_dest->simm;

	int16_t result=(int16_t)op_src->simm - dest;


//OF
	if(op_src->simm>0&&dest<0&&result<0)
		cpu.OF=1;
	else if(op_src->simm<0&&dest>0&&result>0)
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

	return 3;
}


make_helper(cmp_i82rm_l)
{
	panic("进去了,译码错误");
	decode_rm_imm_w(eip);
	int8_t dest=op_dest->simm;
	int32_t result=op_src->simm-dest;
Log("cmp操作数为%d  %d",op_src->simm,dest);

//OF
	if(op_src->simm>0&&dest<0&&result<0)
		cpu.OF=1;
	else if(op_src->simm<0&&dest>0&&result>0)
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

	return 3;
}


make_helper_v(cmp_i2a)
make_helper_v(cmp_i2rm)
make_helper_v(cmp_i82rm)
make_helper_v(cmp_r2rm)
make_helper_v(cmp_rm2r)