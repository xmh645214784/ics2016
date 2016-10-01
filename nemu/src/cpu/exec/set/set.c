#include "cpu/exec/helper.h"
extern CPU_state cpu;
make_helper(seta)
{
	decode_rm2r_b(eip);
	if(cpu.CF==0&&cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setae)
{
	decode_rm2r_b(eip);
	if(cpu.CF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setb)
{
	decode_rm2r_b(eip);
	if(cpu.CF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}


make_helper(setbe)
{
	decode_rm2r_b(eip);
	if(cpu.CF==1||cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setc)
{
	decode_rm2r_b(eip);
	if(cpu.CF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(sete)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setg)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==0&&cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setge)
{
	decode_rm2r_b(eip);
	if(cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setl)
{
	decode_rm2r_b(eip);
	if(cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setle)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==1||cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setna)
{
	decode_rm2r_b(eip);
	if(cpu.CF==1||cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setnae)
{
	decode_rm2r_b(eip);
	if(cpu.CF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setnb)
{
	decode_rm2r_b(eip);
	if(cpu.CF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setnbe)
{
	decode_rm2r_b(eip);
	if(cpu.CF==0&&cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setnc)
{
	decode_rm2r_b(eip);
	if(cpu.CF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setne)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setng)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==1||cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setnge)
{
	decode_rm2r_b(eip);
	if(cpu.SF!=cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setnl)
{
	decode_rm2r_b(eip);
	if(cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}


make_helper(setnle)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==0&&cpu.SF==cpu.OF)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setno)
{
	decode_rm2r_b(eip);
	if(cpu.OF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setnp)
{
	decode_rm2r_b(eip);
	if(cpu.PF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setns)
{
	decode_rm2r_b(eip);
	if(cpu.SF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setnz)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}







make_helper(seto)
{
	decode_rm2r_b(eip);
	if(cpu.OF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setp)
{
	decode_rm2r_b(eip);
	if(cpu.PF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}

make_helper(setpe)
{
	decode_rm2r_b(eip);
	if(cpu.PF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setpo)
{
	decode_rm2r_b(eip);
	if(cpu.PF==0)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(sets)
{
	decode_rm2r_b(eip);
	if(cpu.SF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
make_helper(setz)
{
	decode_rm2r_b(eip);
	if(cpu.ZF==1)
		write_operand_b(op_src, 1);
	else
		write_operand_b(op_src, 0);
	return 2;
}
