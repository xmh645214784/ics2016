#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"

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
	*base=sgds.base_15_0|(((uint32_t)sgds.base_23_16<<16))|(((uint32_t)sgds.base_31_24<<24));
	*limit=sgds.limit_15_0|((uint32_t)sgds.limit_19_16<<16);
	*granularity=sgds.granularity;
#ifdef DEBUG
	Assert(sgds.present,"sgds failed");
#endif 

}



lnaddr_t seg_translate(swaddr_t addr, size_t len,uint8_t sreg)
{
	if(cpu.cr0.protect_enable==1)
	{
		
	    lnaddr_t base,limit;
	    bool granularity;
	    get_base_and_limit(sreg,&base,&limit,&granularity);
#ifdef DEBUG   
	    if (addr > limit*(granularity?1<<12:1) || addr + len - 1 > limit*(granularity?1<<12:1)) {
	    	panic("can't do segment-translation");
	    }
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
	 * load segment
	 */
	cpu.segment_reg[index].selector = selector;
	cpu.segment_reg[index].segdesc_cache = desc;
}