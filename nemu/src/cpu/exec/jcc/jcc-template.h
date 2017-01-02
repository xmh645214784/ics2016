#include "cpu/exec/template-start.h"
#define make_helper_jcc(instr) make_helper(concat3(instr,_rel_,SUFFIX))



make_helper_jcc(ja)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==0&&cpu.ZF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("ja_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jae)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jae_rel_"str(SUFFIX)" %s",op_src->str);
	
	return 1+DATA_BYTE;
}

make_helper_jcc(jb)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jb_rel_"str(SUFFIX)" %s",op_src->str);
	
	return 1+DATA_BYTE;
}


make_helper_jcc(jbe)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==1||cpu.ZF==1)
	{
		cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jbe_rel_"str(SUFFIX)" %s",op_src->str);	
	return 1+DATA_BYTE;
}

make_helper_jcc(jc)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jc_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}


#if DATA_BYTE==1
make_helper_jcc(jcxz)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.cx==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jcxz_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jecxz)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ecx==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jecxz_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
#endif

make_helper_jcc(je)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("je_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jg)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==0&&cpu.SF==cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jg_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jge)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.SF==cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jge_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jl)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.SF!=cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jl_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jle)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==1||cpu.SF!=cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jle_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jna)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==1||cpu.ZF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jna_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jnae)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnae_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jnb)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnb_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jnbe)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==0&&cpu.ZF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnbe_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jnc)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.CF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnc_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jne)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jne_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jng)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==1||cpu.SF!=cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jng_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jnge)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.SF!=cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnge_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jnl)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.SF==cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnl_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}


make_helper_jcc(jnle)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==0&&cpu.SF==cpu.OF)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnle_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jno)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.OF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jno_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jnp)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.PF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnp_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jns)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.SF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jns_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jnz)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jnz_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}


make_helper_jcc(jo)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.OF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jo_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jp)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.PF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jp_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

make_helper_jcc(jpe)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.PF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jpe_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jpo)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.PF==0)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jpo_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(js)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.SF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("js_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}
make_helper_jcc(jz)
{
	concat(decode_rel_,SUFFIX)(eip+1);
	if(cpu.ZF==1)
	{
	cpu.eip+=(DATA_TYPE_S)op_src->val;
#if DATA_BYTE==2
	cpu.eip=cpu.eip&0xFFFF;
#endif
	}
	print_asm("jz_rel_"str(SUFFIX)" %s",op_src->str);
	return 1+DATA_BYTE;
}

#include "cpu/exec/template-end.h"

#ifdef make_helper_jcc
#undef make_helper_jcc
#endif