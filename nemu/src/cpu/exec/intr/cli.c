#include "cpu/exec/helper.h"

make_helper(cli)
{
	cpu.IF=0;
	return 1;
}