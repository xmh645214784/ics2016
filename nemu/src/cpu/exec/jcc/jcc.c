#include "cpu/exec/helper.h"
#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

make_helper_v(ja_rel);

make_helper_v(jae_rel);

make_helper_v(jb_rel);

make_helper_v(jbe_rel);

make_helper_v(jc_rel);

//make_helper_v(jcxz_rel);

//make_helper_v(jeczx_rel);

make_helper_v(je_rel);

make_helper_v(jg_rel);

make_helper_v(jge_rel);

make_helper_v(jl_rel);

make_helper_v(jle_rel);

make_helper_v(jna_rel);

make_helper_v(jnae_rel);

make_helper_v(jnb_rel);

make_helper_v(jnbe_rel);

make_helper_v(jnc_rel);

make_helper_v(jne_rel);

make_helper_v(jng_rel);

make_helper_v(jnge_rel);

make_helper_v(jnl_rel);

make_helper_v(jnle_rel);

make_helper_v(jno_rel);

make_helper_v(jnp_rel);

make_helper_v(jns_rel);

make_helper_v(jnz_rel);

make_helper_v(jo_rel);

make_helper_v(jp_rel);

make_helper_v(jpe_rel);

make_helper_v(jpo_rel);

make_helper_v(js_rel);

make_helper_v(jz_rel);
