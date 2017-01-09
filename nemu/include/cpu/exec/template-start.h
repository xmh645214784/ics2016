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

#define MEM_R(addr,seg) swaddr_read(addr, DATA_BYTE,seg)
#define MEM_W(addr, data,seg) swaddr_write(addr, DATA_BYTE, data,seg)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))


#define CPU_AFFECT_OF(src,des,isADD) \
do\
{\
if(isADD)\
{\
	DATA_TYPE_S sum=(DATA_TYPE_S)(src)+(DATA_TYPE_S)(des);\
	long long a_=(DATA_TYPE_S)(src);\
	long long b_=(DATA_TYPE_S)(des);\
	long long sum2=a_+b_;\
	cpu.OF=sum!=sum2;\
}\
else\
{\
	long long a_=(DATA_TYPE_S)(src);\
	long long b_=(DATA_TYPE_S)(des);\
	long long sum2=b_-a_;\
	DATA_TYPE_S sum=-(DATA_TYPE_S)(src)+(DATA_TYPE_S)(des);\
	cpu.OF=sum!=sum2;\
}\
}while(0);

#define CPU_AFFECT_SF(src,des,isADD) \
do\
{\
	if(isADD)\
	{\
		cpu.SF=MSB((DATA_TYPE_S)(src)+(DATA_TYPE_S)(des));\
	}\
	else\
	{\
		cpu.SF=MSB(-(DATA_TYPE_S)(src)+(DATA_TYPE_S)(des));\
	}\
}while(0);

#define CPU_AFFECT_ZF(src,des,isADD) \
do\
{\
	if(isADD)\
	{\
		cpu.ZF=(DATA_TYPE_S)((DATA_TYPE_S)(src)+(DATA_TYPE_S)(des))==0;\
	}\
	else\
	{\
		cpu.ZF=(DATA_TYPE_S)(-(DATA_TYPE_S)(src)+(DATA_TYPE_S)(des))==0;\
	}\
}while(0);

//PF
#define CPU_AFFECT_PF(src,des,isADD) \
do\
{\
	if(isADD)\
		cpu.PF=is_even_number_of_1((src)+(des));\
	else\
		cpu.PF=is_even_number_of_1(-(src)+(des));\
}while(0);



#define CPU_AFFECT_CF(src,des,isADD) \
do\
{\
	if(isADD)\
	{\
		DATA_TYPE sum=(DATA_TYPE_S)(src)+(DATA_TYPE_S)(des);\
		if(sum>=(src)&&sum>=(des))\
			cpu.CF=0;\
		else\
			cpu.CF=1;\
	}\
	else\
	{\
		DATA_TYPE src__neg=(src);\
		if((des)>=src__neg)\
			cpu.CF=0;\
		else\
			cpu.CF=1;\
	}\
}while(0);

