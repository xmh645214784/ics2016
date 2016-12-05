#include "cpu/exec/helper.h"
make_helper(popa)
{
#ifdef DEBUG
	assert(ops_decoded.is_operand_size_16 == 0);
#endif
    
    cpu.edi=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;

    cpu.esi=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;

    cpu.ebp=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;

    //ski esp
    cpu.esp+=4;

    cpu.ebx=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;

    cpu.edx=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;

    cpu.ecx=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;  

    cpu.eax=swaddr_read(cpu.esp,4,SR_SS);
    cpu.esp+=4;

    return 1;
}