#include "cpu/exec/helper.h"

make_helper(hlt)
{
	//closing intr state
	if(cpu.IF==0)
		assert(0);
	while(cpu.INTR==0)
	{
		//??????????????????
#ifdef DEBUG
		Log("in hlt");
#endif
	}
#ifdef DEBUG	
	Log("out hlt");
#endif
	return 1;
}