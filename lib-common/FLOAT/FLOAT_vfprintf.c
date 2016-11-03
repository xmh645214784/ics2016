#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include "FLOAT.h"

extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern char _ppfs_setargs;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);

__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f) {
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */

	char buf[80];
	int len;
	if(f>>31==0)// +
	{
		int integer=f>>16;
		int point_sum=0;
		int i;
		for(i=0;i<16;i++)
		{
			int point=(f>>(15-i))&1;
			point_sum=point_sum*2+point;
		}
		for(i=0;i<6;i++)
			point_sum*=5;
		point_sum/=1024;
		len=sprintf(buf,"%d.%06d",integer,point_sum);
	}
	else//-
	{
		f=-f;
		int integer=f>>16;
		int point_sum=0;
		int i;
		for(i=0;i<16;i++)
		{
			int point=(f>>(15-i))&1;
			point_sum=point_sum*2+point;
		}
		for(i=0;i<6;i++)
			point_sum*=5;
		point_sum/=1024;
		len=sprintf(buf,"%d.%06d",-integer,point_sum);
	}

//	int len = sprintf(buf, "0x%08x", f);
	return __stdio_fwrite(buf, len, stream);
}

static void modify_vfprintf() {
	/* TODO: Implement this function to hijack the formating of "%f"
	 * argument during the execution of `_vfprintf_internal'. Below
	 * is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */
	 //printf("%08x %08x\n", &_fpmaxtostr,&_vfprintf_internal);
	
//mprotect((void *)(((unsigned int)&_vfprintf_internal + 0x306 - 100) & 0xfffff000), 4096*2, PROT_READ | PROT_WRITE | PROT_EXEC);
	 int a=*(int *)(&_vfprintf_internal+775);
	 //printf("before hijack:%08x\n",*(int *)(&_vfprintf_internal+775));
	 //printf("old jup=%d  %08x\n",4+(int)(&_vfprintf_internal+775)+a,4+(int)(&_vfprintf_internal+775)+a);
	 //printf("format_FLOAT-_fpmaxtostr=%d %08x\n",(int)&format_FLOAT-(int)&_fpmaxtostr,(int)&format_FLOAT-(int)&_fpmaxtostr);
	 *(int *)(&_vfprintf_internal+775)=(a+(int)&format_FLOAT-(int)&_fpmaxtostr);
	 //printf("after hijack:%d %08x\n",*(int *)(&_vfprintf_internal+775),*(int *)(&_vfprintf_internal+775));
	 //printf("hijack jump=%08x\n\n\n\n",(int)&_vfprintf_internal+775+*(int *)(&_vfprintf_internal+775)+4);
	 

	 //PUSH (%EDX) ff32   NOP 90

	 *(char *)(&_vfprintf_internal+764)=0xFF;
	 *(char *)(&_vfprintf_internal+765)=0x32;
	 *(char *)(&_vfprintf_internal+766)=0x90;

	 //to fix stacksize
	 *(char *)(&_vfprintf_internal+763)=0x08;

	 //other float instruction
	 *(char *)(&_vfprintf_internal+740)=0x90;
	 *(char *)(&_vfprintf_internal+741)=0x90;

	 *(char *)(&_vfprintf_internal+744)=0x90;
	 *(char *)(&_vfprintf_internal+745)=0x90;

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

}

static void modify_ppfs_setargs() {
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */
	 *(char *)(&_ppfs_setargs+113)=0xeb;
	 *(char *)(&_ppfs_setargs+114)=0x30;

#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif

}

void init_FLOAT_vfprintf() {
	modify_vfprintf();
	modify_ppfs_setargs();
}
