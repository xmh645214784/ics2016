#include "cpu/exec/helper.h"
make_helper(iretd)
{
	if(cpu.cr0.protect_enable==0)
	{
		if(ops_decoded.is_operand_size_16)
		{
			// Log("need notice")
			// cpu.ip=swaddr_read(cpu.esp,2,SR_SS);
			// //BUG?
			// cpu.esp+=2;
			assert(0);
		}
		else
		{
			//EIP
			cpu.eip=swaddr_read(cpu.esp,4,SR_SS);
			cpu.esp+=4;
			
			//CS
			Selector temp;
			temp.val=swaddr_read(cpu.esp,4,SR_SS);
extern void load_segment(int index,Selector selector);
			load_segment(SR_CS,temp);
			cpu.esp+=4;
			
			//EFLAGS
			cpu.EFLAGS=swaddr_read(cpu.esp,4,SR_SS);
			cpu.esp+=4;
		}
	}
	else
	{
		//EFLAGS
		cpu.EFLAGS=swaddr_read(cpu.esp+8,4,SR_SS);
		
		//EIP
		cpu.eip=swaddr_read(cpu.esp,4,SR_SS);
		cpu.esp+=4;

		//CS
		Selector temp;
		temp.val=swaddr_read(cpu.esp,4,SR_SS);
extern void load_segment(int index,Selector selector);
		load_segment(SR_CS,temp);
		cpu.esp+=4;		

//throwaway ← Pop(); (* pop away EFLAGS already read *)
		cpu.esp-=4;

		//ES
		temp.val=swaddr_read(cpu.esp,4,SR_SS);
		load_segment(SR_ES,temp);
		cpu.esp+=4;		

		//DS
		temp.val=swaddr_read(cpu.esp,4,SR_SS);
		load_segment(SR_DS,temp);
		cpu.esp+=4;	

		//FS ??????
		temp.val=swaddr_read(cpu.esp,4,SR_SS);
		load_segment(SR_DS,temp);
		cpu.esp+=4;	
		assert(0);	

	}
	print_asm("iretd");
	return 0;
}