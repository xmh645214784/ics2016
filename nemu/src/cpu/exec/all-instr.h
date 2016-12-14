#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"


//my
#include "call/call.h"
#include "push/push.h"
#include "test/test.h"
#include "jcc/jcc.h"
#include "cmp/cmp.h"


#include "pop/pop.h"
#include "ret/ret.h"
make_helper(leave_v);
#include "data-mov/movsx.h"
#include "data-mov/movzx.h"
#include "data-mov/cwdcdq.h"


#include "arith/add.h"
#include "arith/adc.h"
#include "arith/sub.h"
#include "arith/sbb.h"

#include "set/set.h"
#include "jmp/jmp.h"

#include "data-mov/movs.h"
#include "data-mov/stos.h"
#include "data-mov/lods.h"

#include "cmp/scas.h"

make_helper(shrdi_v);

#include "loop/loop.h"

make_helper(lgdt_rm2r_l);
make_helper(lidt_rm2r_l);

make_helper(std);

make_helper(cld);

#include "intr/int.h"
make_helper(cli);

make_helper(pusha);
make_helper(popa);

make_helper(iretd);

#include "inout/in.h"
#include "inout/out.h"

make_helper(sti);

make_helper(hlt);

#include "arith/bt.h"

make_helper(cbw_v);