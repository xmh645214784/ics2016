#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(seta)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==0&&cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("seta %s",op_src->str);
	return 1+len;
}

make_helper(setae)
{
	int  len=decode_rm2r_b(eip+1);
	if(cpu.CF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setae %s",op_src->str);
	return 1+len;
}

make_helper(setb)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setb %s",op_src->str);
	return 1+len;
}


make_helper(setbe)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==1||cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setbe %s",op_src->str);
	return 1+len;
}

make_helper(setc)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setc %s",op_src->str);
	return 1+len;
}

make_helper(sete)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("sete %s",op_src->str);
	return 1+len;
}
make_helper(setg)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==0&&cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setg %s",op_src->str);
	return 1+len;
}
make_helper(setge)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setge %s",op_src->str);
	return 1+len;
}

make_helper(setl)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setl %s",op_src->str);
	return 1+len;
}
make_helper(setle)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==1||cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setle %s",op_src->str);
	return 1+len;
}
make_helper(setna)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==1||cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setna %s",op_src->str);
	return 1+len;
}
make_helper(setnae)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnae %s",op_src->str);
	return 1+len;
}
make_helper(setnb)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnb %s",op_src->str);
	return 1+len;
}
make_helper(setnbe)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==0&&cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnbe %s",op_src->str);
	return 1+len;
}
make_helper(setnc)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.CF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnc %s",op_src->str);
	return 1+len;
}
make_helper(setne)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setne %s",op_src->str);
	return 1+len;
}

make_helper(setng)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==1||cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setng %s",op_src->str);
	return 1+len;
}

make_helper(setnge)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnge %s",op_src->str);
	return 1+len;
}
make_helper(setnl)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnl %s",op_src->str);
	return 1+len;
}


make_helper(setnle)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==0&&cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnle %s",op_src->str);
	return 1+len;
}

make_helper(setno)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.OF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setno %s",op_src->str);
	return 1+len;
}

make_helper(setnp)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.PF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnp %s",op_src->str);
	return 1+len;
}

make_helper(setns)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.SF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setns %s",op_src->str);
	return 1+len;
}

make_helper(setnz)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setnz %s",op_src->str);
	return 1+len;
}







make_helper(seto)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.OF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("seto %s",op_src->str);
	return 1+len;
}
make_helper(setp)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.PF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setp %s",op_src->str);
	return 1+len;
}

make_helper(setpe)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.PF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setpe %s",op_src->str);
	return 1+len;
}
make_helper(setpo)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.PF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setpo %s",op_src->str);
	return 1+len;
}
make_helper(sets)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.SF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("sets %s",op_src->str);
	return 1+len;
}
make_helper(setz)
{
	int len=decode_rm2r_b(eip+1);
	if(cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	print_asm("setz %s",op_src->str);
	return 1+len;
}
