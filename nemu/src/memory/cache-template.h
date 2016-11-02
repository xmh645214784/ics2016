#include "macro.h"
#include "common.h"
#define BLOCK_SIZE (1<<LOG2_BLOCK_SIZE)
#define CACHE_SIZE (1<<LOG2_CACHE_SIZE)
#define WAY (1<<LOG2_WAY)

#define LOG2_NR_GROUP (LOG2_CACHE_SIZE-LOG2_BLOCK_SIZE-LOG2_WAY)


#define ADDRNOTE (32-LOG2_NR_GROUP-LOG2_BLOCK_SIZE)
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


#define CACHE_OBJECT concat3(l,LEVEL,cache)


/*For Initialize*/
/*prototype: init_L1Cache*/
void concat(init_,CACHE_NAME)()
{
	Assert((1<<LOG2_NR_GROUP)==NR_GROUP,"group index caculate failed");

	/*CACHE*/
	int i=0;
	for(;i<NR_OF_CACHELINE;i++)
		CACHE_OBJECT.cacheline[i].valid=0;
	/*prototype l1cache l2cache*/
}



int concat(read_,CACHE_NAME)(void *dest,hwaddr_t addr,size_t len)
{
	int i;
	uint32_t groupindex=((addr>>LOG2_BLOCK_SIZE)%NR_GROUP)*NR_GROUP;
	/*each group first element's index*/

	Assert(groupindex>=0&&groupindex<NR_GROUP-WAY,"group index caculate failed");
	for(i=0;i<WAY;i++)
	{
		if(CACHE_OBJECT.cacheline[groupindex+i].valid==1&&CACHE_OBJECT.cacheline[groupindex+i].addrnote==(addr>>(LOG2_NR_GROUP+LOG2_BLOCK_SIZE)))
		{
			memcpy(dest,&CACHE_OBJECT.cacheline[groupindex+i].data[addr%BLOCK_SIZE],len);
			return 1;
		}
	}

	return 0;
}



#undef CACHE_OBJECT

#undef LEVEL


#undef WAY
#undef BLOCK_SIZE
#undef ADDRNOTE
#undef NR_OF_CACHELINE
#undef NR_GROUP


#undef LOG2_CACHE_SIZE
#undef LOG2_BLOCK_SIZE
#undef LOG2_WAY
#undef LOG2_NR_GROUP


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