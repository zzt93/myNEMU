#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(concat(cmp_AXi_, SUFFIX)) {
    DATA_TYPE reg = REG(AX);
    //TODO examine the signed extend
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    DATA_TYPE tmp = reg - imm;
    set_zf(tmp);
    set_pf(tmp);
    set_sf(tmp);
    //use carry bit as borrow bit
    set_cf_b(reg, imm);
    set_af_b(reg, imm);
    set_of_minus(DATA_BYTE, tmp, reg, imm);
    // 1 means opcode, DATA_BYTE means the immediate number
    return DATA_BYTE + 1;
}

#include "exec/template-end.h"