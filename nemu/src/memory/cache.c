/*level 1 cache*/
#define LOG2_BLOCK_SIZE (6)
#define LOG2_WAY (3)
#define CACHE_SIZE (64*1024)
#define CACHE_NAME L1Cache
#define NOT_WRITE_ALLOCATE
#define LEVEL 1

#include "cache-template.h"

#define LOG2_BLOCK_SIZE (6)
#define LOG2_WAY (3)
#define CACHE_SIZE (64*1024)
#define CACHE_NAME L2Cache
#define WRITE_ALLOCATE
#define LEVEL 2

#include "cache-template.h"