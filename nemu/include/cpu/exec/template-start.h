#include "cpu/exec/helper.h"
extern CPU_state cpu;
bool is_even_number_of_1(uint32_t val);
#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]

#define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))

//下 need signed

#define CPU_AFFECT_OF(src,des) \
	if(src>0&&des>0&&src+des<0)\
		cpu.OF=1;\
	else if(src<0&&des<0&&src+des>0)\
		cpu.OF=1;\
	else\
		cpu.OF=0;

#define CPU_AFFECT_SF(src,des) \
	if(src+des>=0)\
		cpu.SF=0;\
	else\
		cpu.SF=1;

#define CPU_AFFECT_ZF(src,des) \
	if(src+des==0)\
		cpu.ZF=1;\
	else\
		cpu.ZF=0;

//PF
#define CPU_AFFECT_PF(src,des) \
	if(is_even_number_of_1(src+des))\
		cpu.PF=1;\
	else\
		cpu.PF=0;

//CF
//
/*
	//可能有BUG
#define CPU_AFFECT_CF(src,des,isADD) \
	long long sum=(DATA_TYPE)src+(DATA_TYPE)des;\
	if((sum>>DATA_BYTE*8)&1)\
		cpu.CF=1;\
	else\
		cpu.CF=0;\
	if(!isADD)\
		cpu.CF=!cpu.CF;
*/

#define CPU_AFFECT_CF(src,des,isADD) \
	if(isADD)\
	{\
		DATA_TYPE sum=(DATA_TYPE)src+(DATA_TYPE)des;\
		if(sum>=src&&sum>=des)\
			cpu.CF=0;\
		else\
			cpu.CF=1;\
	}\
	else\
	{\
		DATA_TYPE src__neg=-src;\
		if(des>=src__neg)\
			cpu.CF=0;\
		else\
			cpu.CF=1;\
	}

		/*
#define CPU_AFFECT_CF(src,des,isADD) \
		DATA_TYPE result__=(DATA_TYPE)src+(DATA_TYPE)des;\
		if(src>result__||des>result__)\
			cpu.CF=1;\
		else\
			cpu.CF=0;*/
