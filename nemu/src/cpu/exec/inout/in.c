#include "cpu/exec/helper.h"
#include "device/port-io.h"
make_helper(in_i2a_b) 
{
	uint8_t len=decode_i_b(eip+1);
	cpu.al =pio_read(op_src->val, 1);
	print_asm("in %s,%%al",op_src->str);
	return 1+len;
}

make_helper(in_i2a_w) {
	uint8_t len=decode_i_b(eip+1);
	cpu.ax =pio_read(op_src->val, 2);
	print_asm("in %s,%%ax",op_src->str);
	return 1+len;
}

make_helper(in_i2a_l) {
	uint8_t len=decode_i_b(eip+1);
	cpu.eax =pio_read(op_src->val, 4);
	print_asm("in %s,%%eax,",op_src->str);
	return 1+len;
}

//////////////////////
make_helper(in_d2a_b) {
	cpu.al = pio_read(cpu.dx, 1);
	print_asm("in (%%dx),%%al");
	return 1;
}
make_helper(in_d2a_w) {
	cpu.ax = pio_read(cpu.dx, 2);
	print_asm("in (%%dx),%%ax");
	return 1;
}

make_helper(in_d2a_l) {
	cpu.eax = pio_read(cpu.dx, 4);
	print_asm("in (%%dx),%%eax");
	return 1;
}

make_helper_v(in_d2a)
make_helper_v(in_i2a)