#include "common.h"
#include "cpu/reg.h"
#include "device/mmio.h"
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
lnaddr_t seg_translate(swaddr_t addr, size_t len,uint8_t sreg);
hwaddr_t page_translate(lnaddr_t addr);
extern uint32_t read_L1Cache(hwaddr_t,size_t);
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//old:return dram_read(addr, len) & (~0u >> ((4 - len) << 3));

		extern long long allCachecount;
		allCachecount++;
	int map_NO = is_mmio(addr);	
	if(map_NO==-1){
		

// printf("\033[1;31;40m hwaddr_read addr=%08x len=%d \033[0m\n",addr,len);
// 	uint32_t temp=read_L1Cache(addr,len)& (~0u >> ((4 - len) << 3));
// // Log("read_L1Cache(addr,len)&(~0u >> ((4 - len) << 3)))=%x\ndram_read(addr, len) & (~0u >> ((4 - len) << 3)))=%x\n",temp,dram_read(addr, len) & (~0u >> ((4 - len) << 3)));
// 	assert(temp==(dram_read(addr, len) & (~0u >> ((4 - len) << 3))));
// 	return temp;
		// 
		// return read_L1Cache(addr,len);
		return (dram_read(addr, len) & (~0u >> ((4 - len) << 3)));
	}
	else
		return mmio_read(addr,len,map_NO);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
//old: dram_write(addr, len, data);
		extern long long allCachecount;
		allCachecount++;


	int map_NO = is_mmio(addr);	
	if(map_NO==-1){

// printf("\033[1;31;40m hwaddr_write addr=%08x len=%d data=%x\033[0m\n",addr,len,data);
		// extern void write_L1Cache(uint32_t src,hwaddr_t addr,size_t len);
		// write_L1Cache(data,addr,len);
		dram_write(addr, len, data);
		// assert(data==read_L1Cache(addr,len));

	}
	else
	{
		mmio_write(addr,len,data,map_NO);
	}
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	if(cpu.cr0.protect_enable==1&&cpu.cr0.paging==1)
	{
		/*unaligned*/
//		if(((addr&0xfff)+len)>(0xfff+1))  /*1 is important*/
//		{
//			int len1=(addr&0xfff)+len-1-0xfff;
//			int len2=len-len1;
//			uint32_t low=(hwaddr_read(page_translate(addr),4)&(~0u >> ((4 - len1) << 3)));
//			uint32_t high=((hwaddr_read(page_translate(addr+len1),4)&(~0u >> ((4 - len2) << 3)))<<(8*len1));
//			return low|high;
//		}
//		else
		  return hwaddr_read(page_translate(addr),len);
	}
	else
		return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	if(cpu.cr0.protect_enable==1&&cpu.cr0.paging==1)
	{
		/*unaligned*/
//		if(((addr&0xfff)+len)>(0xfff+1))  /*1 is important*/
//		{
			// int len1=(addr&0xfff)+len-1-0xfff;
			// int len2=len-len1;
			// uint32_t write1,write2;
			// switch(len1)
			// {
			// case 1:write1=data&0xff;break;
			// case 2:write1=data&0xffff;break;
			// case 3:write1=data&0xffffff;break;
			// default: assert(0);
			// }
			// switch(len2)
			// {
			// case 1:write2=(data>>8*len1)&0xff;break;
			// case 2:write2=(data>>8*len1)&0xffff;break;
			// case 3:write2=(data>>8*len1)&0xffffff;break;
			// default: assert(0);
			// }
			// hwaddr_write(page_translate(addr),len1,write1);
			// hwaddr_write(page_translate(addr+len1),len2,write2);
//			int i=0;
//			for(;i<len;i++)
//			{
//				hwaddr_write(page_translate(addr+i),1,(uint8_t)data);
//				data=data>>8;
//			}
//		}
//		else
			hwaddr_write(page_translate(addr),len,data);
	}
	else
		hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

