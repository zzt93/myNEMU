#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"


make_helper(concat(test_iAX_, SUFFIX)) {
    DATA_TYPE reg = REG(AX);
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    DATA_TYPE tmp = reg & imm;
    cpu.of = 0;
    cpu.cf = 0;
    set_zf(tmp);
    set_sf(tmp);
    set_pf(tmp);

    print_asm("test" str(SUFFIX) " %%%s, $0x%x", REG_NAME(AX), imm);
    return DATA_BYTE + 1;
}


make_helper(concat(test_r&rm_, SUFFIX)) {
    ModR_M m;
    DATA_TYPE imm;
    m.val = instr_fetch(eip + 1, 1);
    if (m.mod == 3) {
        
    } else {
        
    }

}
#include "exec/template-end.h"