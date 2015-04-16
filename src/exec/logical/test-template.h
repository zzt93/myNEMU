#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"


make_helper(concat(test_iAX_, SUFFIX)) {
    DATA_TYPE reg = REG(AX);
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    DATA_TYPE tmp = reg & imm;
    set_flag(tmp, DATA_BYTE);

    print_asm("test" str(SUFFIX) " %%%s, $0x%x", REG_NAME(AX), imm);
    return DATA_BYTE + 1;
}


make_helper(concat(test_r7rm_, SUFFIX)) {
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    if (m.mod == 3) {
        DATA_TYPE reg = REG(m.reg);
        DATA_TYPE r = REG(m.R_M);
        DATA_TYPE tmp = reg & r;
        set_flag(tmp, DATA_BYTE);
        
        print_asm("test" str(SUFFIX) " %%%s, %%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
        return 2;
    } else {
        swaddr_t addr;
        int len = read_ModR_M(eip + 1, &addr);//interpret ModR_M
        DATA_TYPE me = MEM_R(addr);
        DATA_TYPE reg = REG(m.reg);
        DATA_TYPE tmp = me & reg;
        set_flag(tmp, DATA_BYTE);
        
        print_asm("test" str(SUFFIX) " %%%s, %s", REG_NAME(m.reg), ModR_M_asm);
        return len + 1;
    }

}
#include "exec/template-end.h"