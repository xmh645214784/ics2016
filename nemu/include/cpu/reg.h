#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../../lib-common/x86-inc/cpu.h"
#include "../../../lib-common/x86-inc/mmu.h"


typedef union 
{
	struct {
	uint32_t RPL:2;
	uint32_t TI :1;
	uint32_t index:13;
	};

	uint16_t val;
}Selector;//segment selector


typedef struct 
{

	Selector selector;

	SegDesc segdesc_cache;
}Segment_reg;



enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum {SR_ES,SR_CS,SR_SS,SR_DS};
/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union
	{
		union  {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		}gpr[8];

		/* Do NOT change the order of the GPRs' definitions. */

		struct 
		{
			union
			{
				uint32_t eax;
				uint16_t ax;
				struct 
				{
					uint8_t al;
					uint8_t ah;
				};
			};
			union
			{
				uint32_t ecx;
				uint16_t cx;
				struct 
				{
					uint8_t cl;
					uint8_t ch;
				};
			};
			union
			{
				uint32_t edx;
				uint16_t dx;
				struct 
				{
					uint8_t dl;
					uint8_t dh;
				};
			};	
			union
			{
				uint32_t ebx;
				uint16_t bx;
				struct 
				{
					uint8_t bl;
					uint8_t bh;
				};
			};			
			union
			{
				uint32_t esp;
				uint16_t sp;
			};
			union
			{
				uint32_t ebp;
				uint16_t bp;
			};
			union
			{
				uint32_t esi;
				uint16_t si;
			};			
			union
			{
				uint32_t edi;
				uint16_t di;
			};		
			
		};
	};

	//EFLAGS
	union
	{
		volatile uint32_t EFLAGS;
		struct 
		{
			volatile uint8_t CF :1;
			volatile uint8_t nodef1:1;
			volatile uint8_t PF :1;
			volatile uint8_t nodef3 :1;
			volatile uint8_t AF :1;
			volatile uint8_t nodef5 :1;
			volatile uint8_t ZF :1;
			volatile uint8_t SF :1;
			volatile uint8_t TF :1;
			volatile uint8_t IF :1;
			volatile uint8_t DF :1;
			volatile uint8_t OF :1;
			volatile uint8_t TOPL:2;
			volatile uint8_t NT :1;
			volatile uint8_t nodef15 :1;
			volatile uint8_t RF :1;
			volatile uint8_t VM :1;
			volatile uint16_t nodef18:14;
		};
	};

	union
	{
		swaddr_t eip;
		uint16_t ip;
	};

/**
 * for segment
 */
	CR0 cr0;
	struct 
	{
		uint16_t gdtrlimit;		
		lnaddr_t gdtr;
	};

	union{
		struct{
			Segment_reg es;
			Segment_reg cs;
			Segment_reg ss;
			Segment_reg ds;
		};
		
		Segment_reg segment_reg[4];
	};
	/**
	 * for page
	 */
	CR3 cr3;

	/**
	 * for interrupt
	 */
	struct 
	{
		uint16_t idtrlimit;		
		lnaddr_t idtr;
	};

	/**
	 * for outdevice intr
	 */
	volatile bool INTR;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* segreg_name[];

#endif
