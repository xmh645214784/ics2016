#include "cpu/exec/helper.h"
make_helper(int_i_b)
{
	int len=decode_i_b(eip+1);
//	assert(0);

extern	void raise_intr(uint8_t NO);
	
/**
 * The INT instruction generates via software a call to an interrupt
handler. The immediate operand, from 0 to 255, gives the index number
into the Interrupt Descriptor Table (IDT) of the interrupt routine to be
called.
 */
#ifdef DEBUG
	assert((int)op_src->val>=0&&(int)op_src->val<=255);
#endif	
	
	cpu.eip+=2;
//This is very important for save info in stack
	print_asm("int 0x%02x",op_src->val);
	raise_intr(op_src->val);
#ifdef DEBUG
	assert(0);
#endif
	return 1+len;
}
/*
make_helper(int3)
{
	void raise_intr(uint8_t NO);
	raise_intr(3);

	print_asm("int 3");
	cpu.eip+=1;
	return 1;
}*/

make_helper(int4)
{
	print_asm("int 4");

//This is very important for save info in stack
	cpu.eip+=1;

extern void raise_intr(uint8_t NO);
	raise_intr(4);

#ifdef DEBUG
	assert(0);
#endif
	return 1;
}