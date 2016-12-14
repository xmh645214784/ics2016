#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#define get_SegDesc_base(segdesc) (segdesc.base_15_0|((uint32_t)segdesc.base_23_16<<16)|((uint32_t)segdesc.base_31_24<<24))
#define get_SegDesc_limit(segdesc) ((segdesc.limit_15_0|((uint32_t)segdesc.limit_19_16<<16))*(segdesc.granularity?(1<<12):1))

void get_base_and_limit(uint8_t sreg,lnaddr_t *base,lnaddr_t * limit,bool *granularity)
{
	uint32_t index_in_descripter_table=cpu.segment_reg[sreg].selector.index;

	uint32_t temp[2];

	/**
	 * little endian
	 */
	temp[0]=lnaddr_read(cpu.gdtr+index_in_descripter_table*8,4);
	temp[1]=lnaddr_read(cpu.gdtr+index_in_descripter_table*8+4,4);

	SegDesc sgds;
	memcpy(&sgds,temp,8);
	*base=get_SegDesc_base(sgds);
	*limit=get_SegDesc_limit(sgds);
#ifdef DEBUG
	Assert(sgds.present,"sgds failed");
#endif 

}


lnaddr_t seg_translate(swaddr_t addr, size_t len,uint8_t sreg)
{
	//printf("sreg=%d\ncr0.protect_enable=%d\n",sreg,cpu.cr0.protect_enable );
	if(cpu.cr0.protect_enable==1)
	{
		/*
	    lnaddr_t base,;
	    bool granularity;
	    get_base_and_limit(sreg,&base,&limit,&granularity);
#ifdef DEBUG   
		lnaddr_t limit;
	    if (addr > limit || addr + len - 1 > limit) {
	    	panic("can't do segment-translation");
	    }
#endif*/
	    SegDesc  temp= cpu.segment_reg[sreg].segdesc_cache;
	    lnaddr_t base=get_SegDesc_base(temp);
	  //  printf("base=%x\n",base );
	    
	   // printf("limit=%x\n",limit );
	//    printf("cpu.cs.cache.granularity=%x\n",cpu.segment_reg[1].segdesc_cache.granularity);

#ifdef DEBUG
	    lnaddr_t limit=get_SegDesc_limit(temp);
	    assert(addr<limit);
#endif
    	return base + addr;

	}
	else
		return addr;
}

/**
 * [load_segment:modify segmentreg showed by param1 by reading selector ]
 * @param index    [seg reg index]
 * @param selector [selector]
 */
void load_segment(int index,Selector selector)
{
#ifdef DEBUG
	assert(index==0||index==1||index==2||index==3);
#endif
	SegDesc desc;
	uint32_t temp[2];

#ifdef DEBUG
	Assert(cpu.gdtrlimit>=selector.index,"selector.index out of range");
#endif
	temp[0] = lnaddr_read(cpu.gdtr + selector.index * 8, 4); // little endian
	temp[1] = lnaddr_read(cpu.gdtr + selector.index * 8 + 4, 4);
	memcpy(&desc, temp, 8);
	
	/**
	 * load segment reg
	 */
	cpu.segment_reg[index].selector = selector;
	cpu.segment_reg[index].segdesc_cache = desc;
}