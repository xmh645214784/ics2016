#include "cpu/exec/helper.h"
#include "device/port-io.h"
make_helper(out_a2i_b)
{
	int len=decode_i_b(eip+1);
	pio_write(op_src->val,1,cpu.al);
	print_asm("out    %%al,(%s)",op_src->str);
	return 1+len;
}

make_helper(out_a2i_w)
{
	int len=decode_i_b(eip+1);
	pio_write(op_src->val,2,cpu.ax);
	print_asm("out    %%ax,(%s)",op_src->str);
	return 1+len;
}

make_helper(out_a2i_l)
{
	int len=decode_i_b(eip+1);
	pio_write(op_src->val,4,cpu.eax);
	print_asm("out    %%eax,(%s)",op_src->str);
	return 1+len;
}

make_helper_v(out_a2i)

make_helper(out_a2d_b)
{
	pio_write(cpu.dx,1,cpu.al);
	print_asm("out    %%al,(%%dx)");
	return 1;
}

make_helper(out_a2d_w)
{
	pio_write(cpu.dx,2,cpu.ax);
	print_asm("out    %%ax,(%%dx)");
	return 1;
}

make_helper(out_a2d_l)
{
	pio_write(cpu.dx,4,cpu.eax);
	print_asm("out    %%eax,(%%dx)");
	return 1;
}

make_helper_v(out_a2d)