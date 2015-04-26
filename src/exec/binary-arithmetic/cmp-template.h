#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(cmp_AXi_, SUFFIX)) {
    DATA_TYPE reg = REG(AX);
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    DATA_TYPE tmp = reg - imm;
    cmp_set_flag(DATA_BYTE, reg, imm, tmp);
    print_asm("cmp" str(SUFFIX) " $0x%x, %%%s", imm, REG_NAME(AX));
    // 1 means opcode, DATA_BYTE means the immediate number
    return DATA_BYTE + 1;
}

make_helper(concat(cmp_i8_rm_, SUFFIX)) {
    ModR_M m;
    //TODO examine the signed extend
    int8_t imm ;
    m.val = instr_fetch(eip + 1, 1);
    if (m.mod == 3) {
        imm = instr_fetch(eip + 1 + 1, 1);
        DATA_TYPE reg = REG(m.R_M);
        DATA_TYPE tmp = reg - imm;
        cmp_set_flag(DATA_BYTE, reg, imm, tmp);
        print_asm("cmp" str(SUFFIX) " $0x%x, %%%s", imm, REG_NAME(m.R_M));
        return 1 + 2; // one for imm, one for opcode, one for ModR_M
    } else {
        swaddr_t addr;
        int len = read_ModR_M(eip + 1, &addr);
        imm = instr_fetch(eip + 1 + len, 1);
        DATA_TYPE me = MEM_R(addr);
        DATA_TYPE tmp = me - imm;
        cmp_set_flag(DATA_BYTE, me, imm, tmp);
        print_asm("cmp" str(SUFFIX) " $0x%x, %s", imm, ModR_M_asm);
        return 1 + len + 1;// one for opcode, one for imm8
    }

}
#include "exec/template-end.h"