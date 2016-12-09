#include "cpu/exec/helper.h"

make_helper(hlt)
{
	//closing intr state
	if(cpu.IF==0)
		assert(0);
	while(cpu.INTR==0)
	{
		;
	}
	return 1;
}