#include "exec/helper.h"
#include "exec/template-start.h"

#if DATA_BYTE == 1

#error unknown DATA_BYTE

#endif

#define SP 4
#define BYTE 8

extern char suffix;

make_helper(concat(push_r_, SUFFIX)) {
    int reg_code = instr_fetch(eip, 1) & 0x7;
    DATA_TYPE to_write;
    
    REG(SP) -= DATA_BYTE * BYTE;
    swaddr_t addr = REG(SP);
    to_write = REG(reg_code);
    if (suffix == 'l' && reg_code == SP) {
        to_write = REG(SP) + DATA_BYTE * BYTE;
    }
    MEM_W(addr, to_write);

    print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));
    return 1;
}

#include "exec/template-end.h"