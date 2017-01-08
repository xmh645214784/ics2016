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
#ifdef WRITE_BACK
	uint32_t dirty :1;
#endif
	uint32_t addrnote;
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
#ifdef CACHE_DEBUG
	Assert((1<<LOG2_NR_GROUP)==NR_GROUP,"group index caculate failed");
#endif
	memset(&CACHE_OBJECT,0,sizeof(CACHE_OBJECT));
	/*prototype l1cache l2cache*/
}

/*each group first element's index*/
#define get_group_index ((addr>>LOG2_BLOCK_SIZE)%NR_GROUP)
#define get_group_index_in_array (((addr>>LOG2_BLOCK_SIZE)%NR_GROUP)*WAY)

/*each addr last LOG2BLOCK_SIZE BIT*/
#define get_offset (addr%BLOCK_SIZE)

/*addrNOTE*/
#define get_addr_note (addr>>(LOG2_NR_GROUP+LOG2_BLOCK_SIZE))


/*find pointer due to addr              */
/*return: data pointer or NULL(not find)*/
static uint8_t * concat(find_data_point_,CACHE_NAME)(hwaddr_t addr) 
{
	int i=0;
	uint32_t group_index_in_array=get_group_index_in_array;
#ifdef CACHE_DEBUG
	Assert(group_index_in_array>=0&&group_index_in_array<=NR_GROUP*WAY-WAY,"group index caculate failed");
#endif
	for(i=0;i<WAY;i++)
	{
		if(CACHE_OBJECT.cacheline[group_index_in_array+i].valid==1&&CACHE_OBJECT.cacheline[group_index_in_array+i].addrnote==get_addr_note)
		{
			// Log("%s find addr is %08x value=%x",str(CACHE_NAME),addr,CACHE_OBJECT.cacheline[group_index_in_array+i].data[get_offset]);

			return &(CACHE_OBJECT.cacheline[group_index_in_array+i].data[get_offset]);
		}
	}
	return NULL;
}


/*
	allocate new cacheline due to addr
	and copy the block into cacheline
	return: result
 */
static inline uint32_t concat(allocate_cacheline_,CACHE_NAME)(hwaddr_t addr,size_t len)
{
	int i;
	uint32_t group_index_in_array=get_group_index_in_array;
#ifdef CACHE_DEBUG
	Assert(group_index_in_array>=0&&group_index_in_array<=NR_GROUP*WAY-WAY,"group index caculate failed");
#endif
	for(i=0;i<WAY;i++)
	{
		//have unused cachline
		if(CACHE_OBJECT.cacheline[group_index_in_array+i].valid==0) 
		{
			//printf("allocate:have unused cacheline\n");
			uint32_t result=dram_read(addr, len)& (~0u >> ((4 - len) << 3));
			CACHE_OBJECT.cacheline[group_index_in_array+i].valid=1;
			CACHE_OBJECT.cacheline[group_index_in_array+i].addrnote=get_addr_note;

			int j;
			for(j=0;j<BLOCK_SIZE;j++)
			{
				CACHE_OBJECT.cacheline[group_index_in_array+i].data[j]=dram_read((get_addr_note<<(LOG2_BLOCK_SIZE+LOG2_NR_GROUP))|(get_group_index<<LOG2_BLOCK_SIZE)|j, 1);
				
			}
#ifdef CACHE_DEBUG
			Assert(((get_addr_note<<(LOG2_BLOCK_SIZE+LOG2_NR_GROUP))|(get_group_index<<LOG2_BLOCK_SIZE)|get_offset)==addr,"for debug");
#endif
			return result;
		}
	}
/////////////////////////////////////////////////
	/*have no unused cachline*/
	/*ti huan!*/
	//printf("allocate:have no unused cacheline and tihuan!\n");
	uint32_t result=dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	#ifdef WRITE_BACK	
		if(CACHE_OBJECT.cacheline[group_index_in_array].dirty==1)
		{
			for(i=0;i<BLOCK_SIZE;i++)
				dram_write((get_addr_note<<(LOG2_NR_GROUP+LOG2_BLOCK_SIZE))+i,1,CACHE_OBJECT.cacheline[group_index_in_array].data[i]);
		}
	#endif

	/**
	 * replace the first element in this group!  not random
	 */
#ifdef CACHE_DEBUG
	Assert(CACHE_OBJECT.cacheline[group_index_in_array].valid==1,"..");
#endif
	CACHE_OBJECT.cacheline[group_index_in_array].addrnote=get_addr_note;

	#ifdef WRITE_ALLOCATE
		CACHE_OBJECT.cacheline[group_index_in_array].dirty=0;
	#endif
	
	int j;
	for(j=0;j<BLOCK_SIZE;j++)
	{
		CACHE_OBJECT.cacheline[group_index_in_array].data[j]=dram_read((get_addr_note<<(LOG2_BLOCK_SIZE+LOG2_NR_GROUP))|(get_group_index<<LOG2_BLOCK_SIZE)|j, 1);
	}
#ifdef CACHE_DEBUG
Assert(addr==((get_addr_note<<(LOG2_BLOCK_SIZE+LOG2_NR_GROUP))|(get_group_index<<LOG2_BLOCK_SIZE)|get_offset),"caculate failed");
#endif	
	return result;

}

	
/*prototype :write_L1Cache write_L2Cache*/
void concat(write_,CACHE_NAME)(uint32_t src,hwaddr_t addr,size_t len)
{
	//Log("write:len=%d addr=%08x",len,addr);
	int i;

#ifdef CACHE_DEBUG
	//Log("group_num=%d  get_group_index_in_array=%d",get_group_index,get_group_index_in_array);
	//Log("NR_GROUP=%d  WAY=%d",NR_GROUP,WAY);

	Assert(group_index_in_array>=0&&group_index_in_array<=NR_GROUP*WAY-WAY,"group index caculate failed");
#endif
	uint8_t *find=concat(find_data_point_,CACHE_NAME)(addr);

	if(find)//HIT
	{	
#if 0
		printf("write:HIT cacheline"str(CACHE_NAME)"\n");
#endif


			concat(CACHE_NAME,count)++;

		//ifdef WRITE_BACK modify the dirty bit
		//group_index_in_array=get_group_index_in_array;
		#ifdef WRITE_BACK			
		uint32_t group_index_in_array=get_group_index_in_array;			
		for(i=0;i<WAY;i++)
			if(CACHE_OBJECT.cacheline[group_index_in_array+i].valid==1&&CACHE_OBJECT.cacheline[group_index_in_array+i].addrnote==get_addr_note)
			{
				CACHE_OBJECT.cacheline[group_index_in_array+i].dirty=1;
				break;
			}
		#endif

		if(addr%BLOCK_SIZE+len<=BLOCK_SIZE)//align_write
		{
			//write cache
			uint32_t temp=src;
			for(i=0;i<len;i++)
			{
				*(find+i)=temp;
				temp>>=8;
			}

			//write dram
			#ifdef WRITE_THROUGH
				dram_write(addr,len,src);
			#endif

			return ;
		}
		else								//unalign_write
		{
			int len1=BLOCK_SIZE-addr%BLOCK_SIZE;//in this cacheline
			int len2=addr%BLOCK_SIZE+len-BLOCK_SIZE;
			concat(write_,CACHE_NAME)(src,addr,len1);
			concat(write_,CACHE_NAME)(src>>(8*len1),addr+len1,len2);
			return ;
		}
	}

////////////////////////////////
	//MISS
#if 0
	printf("write:MISS cacheline"str(CACHE_NAME)"\n");
#endif

#if LEVEL==1
	write_L2Cache(src,addr,len);
#elif LEVEL==2
	#ifdef WRITE_ALLOCATE
	//first update
		dram_write(addr, len,src);
	//allocate a new cacheline
	
//why????
		// concat(allocate_cacheline_,L1Cache)(addr,len);
		concat(allocate_cacheline_,CACHE_NAME)(addr,len);
	#endif
	
	#ifdef NOT_WRITE_ALLOCATE
		dram_write(addr, len,src);
	#endif

#else
		assert(0);
#endif

	
	return;
}






/*prototype :read_L1Cache read_L2Cache*/
uint32_t concat(read_,CACHE_NAME)(hwaddr_t addr,size_t len)
{
	//Log("read:len=%d addr=%08x",len,addr);
	//Log("group_num=%d  get_group_index_in_array=%d",get_group_index,get_group_index_in_array);
	//Log("%d  %d",NR_GROUP,WAY);
#ifdef CACHE_DEBUG
	uint32_t group_index_in_array=get_group_index_in_array;
	Assert(group_index_in_array>=0&&group_index_in_array<=NR_GROUP*WAY-WAY,"group index caculate failed");
#endif
	/*each group first element's index*/
	uint8_t *find=concat(find_data_point_,CACHE_NAME)(addr);
	
	
	//HIT
	if(find)
	{
#if 0
		Log("read :HIT cacheline "str(CACHE_NAME));
#endif

			concat(CACHE_NAME,count)++;

		if(addr%BLOCK_SIZE+len<=BLOCK_SIZE)//align_read
		{
			switch(len)
			{
				case 4:return unalign_rw(find,4);
				case 1:return unalign_rw(find,1);
				case 2:return unalign_rw(find,2);
				case 3:return unalign_rw(find,3);
				default:Assert(0,"switch");
			}
			
		}
		else//unalign_read
		{
			int len1=BLOCK_SIZE-addr%BLOCK_SIZE;//in this cacheline
			int len2=addr%BLOCK_SIZE+len-BLOCK_SIZE;
			uint32_t temp_;
			switch(len1)
			{
				case 4:temp_= unalign_rw(find,4);break;
				case 1:temp_= unalign_rw(find,1);break;
				case 2:temp_= unalign_rw(find,2);break;
				case 3:temp_= unalign_rw(find,3);break;
				default:Assert(0,"switch");
			}
				

			return temp_|((concat(read_,CACHE_NAME)(addr+len1,len2))<<(8*len1));
		}
	}
	//MISS
	else
	{
		#if LEVEL==1
			return read_L2Cache(addr,len);
		#elif LEVEL==2
				#if 0
				printf("read:MISS cacheline "str(CACHE_NAME)"\n");
				#endif
	/*BUG?>?*/	allocate_cacheline_L1Cache(addr,len);


		return concat(allocate_cacheline_,CACHE_NAME)(addr,len);
		#else
		assert(0);
		#endif
	}

}


void concat(debug_,CACHE_NAME) (uint32_t addr)
{
	uint32_t group_index_in_array=get_group_index_in_array;
#ifdef CACHE_DEBUG
	Assert(group_index_in_array>=0&&group_index_in_array<=NR_GROUP*WAY-WAY,"group index caculate failed");
#endif
	/*each group first element's index*/

	int i=0;
	for(i=0;i<WAY;i++)
	{
		if(CACHE_OBJECT.cacheline[group_index_in_array+i].valid==1&&CACHE_OBJECT.cacheline[group_index_in_array+i].addrnote==get_addr_note)
		{
			Log("HIT");
			 Log("%02x",CACHE_OBJECT.cacheline[group_index_in_array+i].data[get_offset]);
			 #ifdef WRITE_BACK
			 	Log("FLAG:valid=%d dirty=%d",1,CACHE_OBJECT.cacheline[group_index_in_array+i].dirty);
			 #endif
			 return ;
		}
	}
	
	Log("MISS");
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


#ifdef NOT_WRITE_ALLOCATE
#undef NOT_WRITE_ALLOCATE 
#endif

#ifdef WRITE_ALLOCATE
#undef WRITE_ALLOCATE 
#endif

#ifdef WRITE_THROUGH
#undef WRITE_THROUGH 
#endif

#ifdef WRITE_BACK
#undef WRITE_BACK 
#endif


#undef get_group_index

#undef get_offset

#undef get_addr_note
