#include "common.h"


uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//old:return dram_read(addr, len) & (~0u >> ((4 - len) << 3));

		extern int allCachecount;
		allCachecount++;

	extern uint32_t read_L1Cache(hwaddr_t,size_t);
#if 0
	printf("\033[1;31;40m hwaddr_read addr=%08x len=%d \033[0m\n",addr,len);
	uint32_t temp=read_L1Cache(addr,len)& (~0u >> ((4 - len) << 3));
	Log("read_L1Cache(addr,len)&(~0u >> ((4 - len) << 3)))=%x\ndram_read(addr, len) & (~0u >> ((4 - len) << 3)))=%x",temp,dram_read(addr, len) & (~0u >> ((4 - len) << 3)));
	assert(temp==(dram_read(addr, len) & (~0u >> ((4 - len) << 3))));
	return temp;
#endif
	return read_L1Cache(addr,len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
//old: dram_write(addr, len, data);
		extern int allCachecount;
		allCachecount++;
	extern void write_L1Cache(uint32_t src,hwaddr_t addr,size_t len);
	write_L1Cache(data,addr,len);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

