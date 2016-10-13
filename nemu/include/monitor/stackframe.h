#ifndef _STACKFRAME_H_
#define _STACKFRAME_H_
	typedef struct {
		swaddr_t prev_ebp;
		swaddr_t ret_addr;
		uint32_t args[4];
		char funcname[20];
		swaddr_t func_addr;
	} PartOfStackFrame;
#endif