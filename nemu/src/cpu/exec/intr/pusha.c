#include "cpu/exec/helper.h"
make_helper(pusha)
{
#ifdef DEBUG
	assert(ops_decoded.is_operand_size_16 == 0);
#endif
    uint32_t temp = cpu.esp;
    
    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.eax,SR_SS);

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.ecx,SR_SS); 

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.edx,SR_SS); 

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.ebx,SR_SS); 

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,temp,SR_SS);

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.ebp,SR_SS);

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.esi,SR_SS);  

    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.edi,SR_SS);  

    return 1;
}