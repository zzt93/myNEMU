#include "exec/helper.h"
#include "exec/template-start.h"


make_helper(concat(je_, SUFFIX)) {
    //have to use the signed integer for we need signed extension
    DATA_TYPE_S rel = instr_fetch(eip + 1, DATA_BYTE);
    JCC(cpu.zf == 1, rel);

    print_asm("je" str(SUFFIX) " $0x%x", rel);
    return DATA_BYTE + 1;
}

#include "exec/template-end.h"

