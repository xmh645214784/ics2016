/*level 1 cache*/
#define LOG2_BLOCK_SIZE (6)
#define LOG2_WAY (3)
#define LOG2_CACHE_SIZE (16)
#define CACHE_NAME L1Cache
#define NOT_WRITE_ALLOCATE
#define LEVEL 1

#include "cache-template.h"

#define LOG2_BLOCK_SIZE (6)
#define LOG2_WAY (4)
#define LOG2_CACHE_SIZE (22)
#define CACHE_NAME L2Cache
#define WRITE_ALLOCATE
#define LEVEL 2

#include "cache-template.h"