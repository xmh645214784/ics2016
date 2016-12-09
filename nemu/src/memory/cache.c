/*level 1 cache*/
#include "common.h"



uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t concat(read_,L1Cache)(hwaddr_t addr,size_t len);
uint32_t concat(read_,L2Cache)(hwaddr_t addr,size_t len);
void concat(write_,L1Cache)(uint32_t src,hwaddr_t addr,size_t len);
void concat(write_,L2Cache)(uint32_t src,hwaddr_t addr,size_t len);


long long allCachecount;
long long L1Cachecount;
long long L2Cachecount; 
/*
	prototype:concat(CACHE_NAME,count)
 */


#define LOG2_BLOCK_SIZE (6)
#define LOG2_WAY (3)
#define LOG2_CACHE_SIZE (16)
#define CACHE_NAME L1Cache
#define WRITE_THROUGH
#define NOT_WRITE_ALLOCATE
#define LEVEL 1

#include "cache-template.h"

#define LOG2_BLOCK_SIZE (6)
#define LOG2_WAY (4)
#define LOG2_CACHE_SIZE (22)
#define CACHE_NAME L2Cache
#define WRITE_BACK
#define WRITE_ALLOCATE
#define LEVEL 2

#include "cache-template.h"