#include "cpu/exec/helper.h"
bool is_even_number_of_1(uint32_t val)
{
	int sumofone=0;
	int i=0;
	for(;i<8;i++)
	{
		if(val%2==1)
			sumofone++;
		val>>=1;
	}
	if(sumofone%2==0)
		return 1;
	else
		return 0;
}
#define DATA_BYTE 1
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "test-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(test_i2a)
make_helper_v(test_i2rm)
make_helper_v(test_r2rm)
