#include "macro.h"
#include "common.h"
#define WAY (1<<LOG2_WAY)
#define BLOCK_SIZE (1<<LOG2_BLOCK_SIZE)
#define ADDRNOTE (32-LOG2_WAY-LOG2_BLOCK_SIZE)
#define NR_OF_CACHELINE (CACHE_SIZE/BLOCK_SIZE)
#define NR_GROUP (NR_OF_CACHELINE/(WAY))


typedef struct 
{
	uint32_t valid :1;
#ifdef WRITEBACK
	uint32_t dirty :1;
#endif
	uint32_t addrnote :ADDRNOTE;
	uint8_t data[BLOCK_SIZE];
}concat(CACHE_NAME,_LINE);

typedef struct 
{
	concat(CACHE_NAME,_LINE) cacheline[NR_OF_CACHELINE];
}CACHE_NAME;

#if LEVEL==1
	CACHE_NAME l1cache;
#elif LEVEL==2
	CACHE_NAME l2cache;
#endif




void concat(init_,CACHE_NAME)()
{
	/*CACHE*/
	int i=0;
	for(;i<NR_OF_CACHELINE;i++)
		concat3(l,LEVEL,cache).cacheline[i].valid=0;
	//prototype l1cache l2cache
}






#undef LEVEL

#undef WAY
#undef BLOCK_SIZE
#undef ADDRNOTE
#undef NR_OF_CACHELINE
#undef NR_GROUP

#undef LOG2_BLOCK_SIZE
#undef LOG2_WAY
#undef CACHE_SIZE
#undef CACHE_NAME

#ifdef WRITEBACK
#undef WRITEBACK
#endif

#ifndef NOT_WRITE_ALLOCATE
#undef NOT_WRITE_ALLOCATE 
#endif

#ifndef WRITE_ALLOCATE
#undef WRITE_ALLOCATE 
#endif